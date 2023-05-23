//**************************************************
// \file ATLTileCalTBSensDet.cc
// \brief: implementation of ATLTileCalTBSensDet
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 6 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBSensDet.hh"
#include "ATLTileCalTBConstants.hh"

//Includers from Geant4
//
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4Poisson.hh"

//Constructor and de-constructor
//
ATLTileCalTBSensDet::ATLTileCalTBSensDet( const G4String& name, const G4String& hitsCollectionName )
    : G4VSensitiveDetector(name),
      fHitsCollection(nullptr) {
  
    collectionName.insert(hitsCollectionName);

}

ATLTileCalTBSensDet::~ATLTileCalTBSensDet() {}

//Intiliaze base method
//
void ATLTileCalTBSensDet::Initialize(G4HCofThisEvent* hce) {
  
    //Create hits collection
    //
    fHitsCollection = new ATLTileCalTBHitsCollection( SensitiveDetectorName, collectionName[0] ); 

    //Add this collection in the hits collection of this event
    //
    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection ); 

    //Allocate hits in hit collection
    //
    auto cellLUT = ATLTileCalTBGeometry::CellLUT::GetInstance();
    for ( std::size_t i=0; i<cellLUT->GetNumberOfCells(); i++ ) {
        fHitsCollection->insert(new ATLTileCalTBHit());
    }

}

//ProcessHits base method
//
G4bool ATLTileCalTBSensDet::ProcessHits( G4Step* aStep, G4TouchableHistory* ) {
  
    //Print out some info step-by-step in sensitive elements
    //
    //G4cout<<"Track #: "<< aStep->GetTrack()->GetTrackID()<< " " <<
    //        "Step #: " << aStep->GetTrack()->GetCurrentStepNumber()<< " "<<
    //        "Volume: " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()<< " " << G4endl;
    //G4cout<<"x: "<< aStep->GetPreStepPoint()->GetPosition().x() <<
    //        "y: "<< aStep->GetPreStepPoint()->GetPosition().y() <<
    //        "z: "<< aStep->GetPreStepPoint()->GetPosition().z() << G4endl;
    //G4cout<<"Particle "<< aStep->GetTrack()->GetParticleDefinition()->GetParticleName()<< " " <<
    //        "Dep(MeV) "<< aStep->GetTotalEnergyDeposit() << " " <<
    //        "Mat "     << aStep->GetPreStepPoint()->GetMaterial()->GetName() << " " << 
    //        "Vol "     << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << G4endl; 

    auto edep = aStep->GetTotalEnergyDeposit();
    if ( edep==0. ) return false; 

    // we only record data within the time window of the digitization
    auto time = aStep->GetPreStepPoint()->GetGlobalTime();
    if ( time > ATLTileCalTBConstants::frame_time_window ) return false;

    auto cellLUT = ATLTileCalTBGeometry::CellLUT::GetInstance();
    auto cellIndex = FindCellIndexFromG4( aStep );
    auto cell = cellLUT->GetCell( cellIndex );
    // Adjust energy according to Birk's Law
    G4double sdep = BirkLaw( aStep );
    // Convert energy to photoelectrons
    sdep = static_cast<G4double>(G4Poisson(ATLTileCalTBConstants::photoelectrons_per_energy * sdep));


    //get local coordinates of PreStepPoint in scintillator
    //
    G4ThreeVector prestepPos = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector localCoord = 
        aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(prestepPos);
    G4double yLocal = localCoord.y();
    G4double zLocal = localCoord.z();
    
    //Apply U-shape and signal separation (up-down)
    //
    G4double sdep_up = 0;
    G4double sdep_down = 0;
    G4int scintillator_copy_no = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(0)->GetCopyNo();
    if (cell.module==ATLTileCalTBGeometry::Module::EXTENDED_C10 ){ 
        //add 6 missing rows for cell C10
        scintillator_copy_no += 6;
    }
    if ( cell.module==ATLTileCalTBGeometry::Module::EXTENDED_D4 ){
        //add 9 missing rows for cell D4
        scintillator_copy_no += 9;
    }
    sdep_up = sdep * Tile_1D_profileRescaled( scintillator_copy_no, yLocal, zLocal, 1, cell/*, 1*/ );
    sdep_down = sdep * Tile_1D_profileRescaled( scintillator_copy_no, yLocal, zLocal, 0, cell/*, 1*/ );

    //Get corresponding hit
    //
    auto hit = (*fHitsCollection)[cellIndex];
    if ( ! hit ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hit from " << cell;
        G4Exception("ATLTileCalTBSensDet::ProcessHits()",
        "MyCode0004", FatalException, msg);
    }         

    //Add hit energy 
    //
    hit->AddEdep(edep);
    hit->AddSdep(time, sdep_up, sdep_down);
    return true;

}

//EndOfEvent base method
//
void ATLTileCalTBSensDet::EndOfEvent(G4HCofThisEvent*) {
    
}

//BrikLaw method
//This method is taken from the ATLAS Athena offline software
//athena/TileCalorimeter/TileG4/TileGeoG4SD/src/TileGeoG4SDCalc.cc
//as on June 2022.
//
G4double ATLTileCalTBSensDet::BirkLaw( const G4Step* aStep ) const {

    /*----------------COMMENT FROM ATHENA---------------*/
    // *** apply BIRK's saturation law to energy deposition ***
    // *** only organic scintillators implemented in this version MODEL=1
    //
    // Note : the material is assumed ideal, which means that impurities
    //        and aging effects are not taken into account
    //
    // algorithm : edep = destep / (1. + RKB*dedx + C*(dedx)**2)
    //
    // the basic units of the coefficient are g/(MeV*cm**2)
    // and de/dx is obtained in MeV/(g/cm**2)
    //
    // exp. values from NIM 80 (1970) 239-244 :
    //
    // RKB = 0.013  g/(MeV*cm**2)  and  C = 9.6e-6  g**2/((MeV**2)(cm**4))
    /*---------------END OF COMMENT FROM ATHENA---------------*/

    const G4double destep = aStep->GetTotalEnergyDeposit() * aStep->GetTrack()->GetWeight();
    const G4Material* material = aStep->GetPreStepPoint()->GetMaterial();
    const G4double charge = aStep->GetPreStepPoint()->GetCharge();

    G4double response = 0.;
    G4double rkb = 0.02002 * CLHEP::g / (CLHEP::MeV * CLHEP::cm2);      //m_birk1 in athena
    G4double m_birk2 = 0.0 * CLHEP::g / (CLHEP::MeV * CLHEP::cm2) * CLHEP::g / (CLHEP::MeV * CLHEP::cm2);

    if ( charge != 0 && aStep->GetStepLength() != 0) {
        //Comment from atlas athena
        // --- correction for particles with more than 1 charge unit ---
        // --- based on alpha particle data (only apply for MODEL=1) ---
        if ( fabs(charge) > 1.0 ) { rkb *= 7.2 / 12.6; }
        const G4double dedx = destep / (aStep->GetStepLength()) / (material->GetDensity());
        response = destep / (1. + rkb * dedx + m_birk2 * dedx * dedx);
    }
    else { response = destep; }

    return response;

}

// FindCellIndexFromG4 method
//
std::size_t ATLTileCalTBSensDet::FindCellIndexFromG4( const G4Step* aStep ) const {
    auto handle = aStep->GetPreStepPoint()->GetTouchableHandle();

    // Get scintillator and period copy number, identical everywhere
    G4int scintillator_copy_no = handle->GetVolume(0)->GetCopyNo();
    G4int period_copy_no = handle->GetVolume(2)->GetCopyNo();

    auto throwGeometryError = [&handle]() -> std::size_t {
        G4ExceptionDescription msg;
        msg << "Fatal during geometry parsing:\n"
            << handle->GetVolume(5)->GetName() << " [" << handle->GetVolume(5)->GetCopyNo() << "] "
            << handle->GetVolume(4)->GetName() << " "
            << handle->GetVolume(3)->GetName() << " "
            << handle->GetVolume(2)->GetName() << " [" << handle->GetVolume(2)->GetCopyNo() << "] "
            << handle->GetVolume(1)->GetName() << " "
            << handle->GetVolume(0)->GetName() << " [" << handle->GetVolume(0)->GetCopyNo() << "] "
            << G4endl;
        G4Exception("ATLTileCalTBSensDet::FindCellIndexFromG4()",
        "MyCode0005", FatalException, msg);
        return SIZE_MAX; // Return impossible size
    };

    // Get module number via string, depends on module layout
    ATLTileCalTBGeometry::Module module;

    auto module_name = handle->GetVolume(5)->GetName();
    if ( module_name == "Tile::BarrelModule" ) {
        switch( handle->GetVolume(5)->GetCopyNo() ) {
            case 1:
                module = ATLTileCalTBGeometry::Module::LONG_LOWER;
                break;
            case 2:
                module = ATLTileCalTBGeometry::Module::LONG_UPPER;
                break;
            default:
                return throwGeometryError();
        }
    }
    else if ( module_name == "EBarrelPos" ) {
        module = ATLTileCalTBGeometry::Module::EXTENDED;
    }
    else if ( module_name == "Tile::Plug2Module" ) {
        module = ATLTileCalTBGeometry::Module::EXTENDED_C10;
    }
    else if ( module_name == "Tile::ITCModule" ) {
        // Tile::Plug1Module has no Tile::AbsorberChild
        module = ATLTileCalTBGeometry::Module::EXTENDED_D4;
    }
    else {
        return throwGeometryError();
    }

    // Get index from CellLUT
    auto cellLUT = ATLTileCalTBGeometry::CellLUT::GetInstance();
    auto index = cellLUT->FindCellIndex(module, scintillator_copy_no, period_copy_no);

    return index;

}

//Tile_1D_profileRescaled method
//This method is adapted from the ATLAS athena offline software
//athena/TileCalorimeter/TileG4/TileGeoG4SD/src/TileGeoG4SDCalc.cc
//as on June 2022.
//
G4double ATLTileCalTBSensDet::Tile_1D_profileRescaled( G4int row, G4double x, G4double y, G4int PMT, ATLTileCalTBGeometry::Cell cell/*, G4int nSide*/ ){

    if (PMT) x *= -1.;

    const double xlow = -0.0495; //dPhi low [rad]
    const double xup = 0.0495; //dPhi up [rad]
    const double range = (xup - xlow); //dPhi range
    const int size = 99;

    const G4double LB_A_TilePMT[size] = { 0.797741, 0.767611, 0.737482, 0.731121, 0.715537, 0.689929, 0.690055, 0.687185,
                                        0.685124, 0.673707, 0.664842, 0.663197, 0.660089, 0.647501, 0.650303, 0.644465,
                                        0.639813, 0.631315, 0.627008, 0.622707, 0.614297, 0.61109, 0.604147, 0.605184,
                                        0.603651, 0.592072, 0.588977, 0.585351, 0.588941, 0.578247, 0.580187, 0.576195,
                                        0.576942, 0.57606, 0.570978, 0.568398, 0.563464, 0.565646, 0.557544, 0.56112,
                                        0.552727, 0.556008, 0.555573, 0.554079, 0.548542, 0.551241, 0.538841, 0.523046,
                                        0.501209, 0.474613, 0.46968, 0.465796, 0.465135, 0.466067, 0.456968, 0.458314,
                                        0.454288, 0.450544, 0.445219, 0.444452, 0.437612, 0.440608, 0.432754, 0.432117,
                                        0.429496, 0.427993, 0.42394, 0.419026, 0.41752, 0.412359, 0.416317, 0.408531,
                                        0.40574, 0.405237, 0.407231, 0.403318, 0.398811, 0.39869, 0.396117, 0.396753,
                                        0.395918, 0.393898, 0.39377, 0.390499, 0.390835, 0.38526, 0.385113, 0.383958,
                                        0.37829, 0.375895, 0.375872, 0.370231, 0.364742, 0.353429, 0.349633, 0.333518,
                                        0.305173, 0.287103, 0.269032 };

    const G4double LB_BC_TilePMT[size] = { 0.83904, 0.781078, 0.723117, 0.708466, 0.691473, 0.680283, 0.673512, 0.668259,
                                         0.663925, 0.661599, 0.66064, 0.645793, 0.638767, 0.638648, 0.633753, 0.632288,
                                         0.62912, 0.621557, 0.610724, 0.611454, 0.608478, 0.598683, 0.599413, 0.59475,
                                         0.591156, 0.585141, 0.58734, 0.582087, 0.581133, 0.577544, 0.565483, 0.565771,
                                         0.566045, 0.562009, 0.558788, 0.554103, 0.554569, 0.552122, 0.55021, 0.544131,
                                         0.546458, 0.545739, 0.542813, 0.539188, 0.539949, 0.531312, 0.529154, 0.511045,
                                         0.50547, 0.485915, 0.483028, 0.469858, 0.475142, 0.468828, 0.471476, 0.466037,
                                         0.462818, 0.460826, 0.456697, 0.450356, 0.451149, 0.447394, 0.444391, 0.440424,
                                         0.437831, 0.436551, 0.433858, 0.429664, 0.424668, 0.429436, 0.426802, 0.422796,
                                         0.423084, 0.416585, 0.416778, 0.413823, 0.413039, 0.407278, 0.409132, 0.405958,
                                         0.400924, 0.40319, 0.402826, 0.400298, 0.39617, 0.393594, 0.388368, 0.387257,
                                         0.390185, 0.383722, 0.377989, 0.373987, 0.368134, 0.36161, 0.353643, 0.340893,
                                         0.31819, 0.31064, 0.30309 };

    const G4double LB_D_TilePMT[size] = { 0.795522, 0.77882, 0.762117, 0.758668, 0.739907, 0.738439, 0.725206, 0.705747,
                                        0.694305, 0.679948, 0.661304, 0.646417, 0.646898, 0.643, 0.64742, 0.642599,
                                        0.644594, 0.630441, 0.62819, 0.633067, 0.620992, 0.615444, 0.614593, 0.605262,
                                        0.600121, 0.593196, 0.598287, 0.594506, 0.587352, 0.586836, 0.584799, 0.565766,
                                        0.573146, 0.555629, 0.556809, 0.554736, 0.548224, 0.548666, 0.547577, 0.547152,
                                        0.548186, 0.541246, 0.534641, 0.535219, 0.527666, 0.526342, 0.519253, 0.514271,
                                        0.497834, 0.486737, 0.478467, 0.476815, 0.470943, 0.470212, 0.465885, 0.463963,
                                        0.457575, 0.456578, 0.457046, 0.45066, 0.446101, 0.442831, 0.437027, 0.435233,
                                        0.431566, 0.423259, 0.432401, 0.419528, 0.426179, 0.421962, 0.415978, 0.416232,
                                        0.416315, 0.406703, 0.406586, 0.405589, 0.405727, 0.400639, 0.397638, 0.399703,
                                        0.389905, 0.38658, 0.388681, 0.381943, 0.380274, 0.371921, 0.367581, 0.359151,
                                        0.360523, 0.359632, 0.35602, 0.351288, 0.350362, 0.348065, 0.341942, 0.337538,
                                        0.323205, 0.306686, 0.290166 };

    /*const G4double EBA_A_TilePMT[size] = { 0.785637, 0.764265, 0.742893, 0.7265, 0.705835, 0.701582, 0.694623, 0.67782,
                                         0.679425, 0.672377, 0.677163, 0.66816, 0.667518, 0.64911, 0.648474, 0.65169,
                                         0.633917, 0.636953, 0.640227, 0.623941, 0.620663, 0.608101, 0.609174, 0.599297,
                                         0.601839, 0.600618, 0.591399, 0.588176, 0.589987, 0.584444, 0.580839, 0.578097,
                                         0.572712, 0.578424, 0.563277, 0.564594, 0.567886, 0.559251, 0.559901, 0.555709,
                                         0.544783, 0.548855, 0.54916, 0.545375, 0.554854, 0.538948, 0.536859, 0.516408,
                                         0.497057, 0.478121, 0.468015, 0.461105, 0.463244, 0.457987, 0.463463, 0.450963,
                                         0.452678, 0.447139, 0.439209, 0.444305, 0.442167, 0.434897, 0.436045, 0.432974,
                                         0.422726, 0.431913, 0.424147, 0.420568, 0.420822, 0.417563, 0.416604, 0.410264,
                                         0.408909, 0.408875, 0.406464, 0.399841, 0.402915, 0.399772, 0.399093, 0.398666,
                                         0.403387, 0.393809, 0.390633, 0.396707, 0.387452, 0.383384, 0.38485, 0.384494,
                                         0.385934, 0.374029, 0.368439, 0.363214, 0.365663, 0.35614, 0.350155, 0.333629,
                                         0.315841, 0.293913, 0.271986 };

    const G4double EBA_BC_TilePMT[size] = { 0.76425, 0.741305, 0.718361, 0.702739, 0.689698, 0.673952, 0.66942, 0.668102,
                                          0.659098, 0.659453, 0.659288, 0.651001, 0.644689, 0.642691, 0.639191,
                                          0.631008, 0.634213, 0.624853, 0.611546, 0.616879, 0.614165, 0.603962,
                                          0.596658, 0.597363, 0.583505, 0.588016, 0.584818, 0.580456, 0.576679,
                                          0.570277, 0.569343, 0.572166, 0.564008, 0.564188, 0.556544, 0.559867,
                                          0.557225, 0.551476, 0.548382, 0.544258, 0.543926, 0.540005, 0.538831,
                                          0.543222, 0.53448, 0.535014, 0.528731, 0.517457, 0.501963, 0.489472, 0.480086,
                                          0.47594, 0.474252, 0.471684, 0.467381, 0.47002, 0.462449, 0.458279, 0.45573,
                                          0.45085, 0.451388, 0.448358, 0.448926, 0.446427, 0.437154, 0.439613, 0.434813,
                                          0.435781, 0.430242, 0.426984, 0.426776, 0.424525, 0.422673, 0.420809,
                                          0.415454, 0.418039, 0.412673, 0.412611, 0.41453, 0.413378, 0.402649, 0.408683,
                                          0.407647, 0.403596, 0.401837, 0.397678, 0.395147, 0.392898, 0.391702,
                                          0.386557, 0.377788, 0.37777, 0.37069, 0.364217, 0.360065, 0.348944, 0.330525,
                                          0.308675, 0.286825 };

    const G4double EBA_D_TilePMT[size] = { 0.83964, 0.797377, 0.755114, 0.758831, 0.750956, 0.727028, 0.710895, 0.7013,
                                         0.696957, 0.678444, 0.675153, 0.659896, 0.656481, 0.6555, 0.648582, 0.64414,
                                         0.634209, 0.63669, 0.630879, 0.626185, 0.610719, 0.616651, 0.608967, 0.597805,
                                         0.601157, 0.599113, 0.589518, 0.581218, 0.582136, 0.577285, 0.576426, 0.566552,
                                         0.565228, 0.554291, 0.552419, 0.555652, 0.545789, 0.54289, 0.537612, 0.537218,
                                         0.539621, 0.528208, 0.529998, 0.530758, 0.532108, 0.522749, 0.511517, 0.51012,
                                         0.499114, 0.490116, 0.480166, 0.474294, 0.463783, 0.464397, 0.467149, 0.460705,
                                         0.452137, 0.446681, 0.449894, 0.44324, 0.438109, 0.437593, 0.433048, 0.437898,
                                         0.429795, 0.425461, 0.428106, 0.42259, 0.42333, 0.417693, 0.416418, 0.413091,
                                         0.412467, 0.415014, 0.408279, 0.402152, 0.406765, 0.403563, 0.395906, 0.399396,
                                         0.396097, 0.395945, 0.387978, 0.389495, 0.386988, 0.382293, 0.374119, 0.367898,
                                         0.366629, 0.359724, 0.358971, 0.353465, 0.352667, 0.348716, 0.352001, 0.340563,
                                         0.319873, 0.320009, 0.320145 };*/

    const G4double EBC_A_TilePMT[size] = { 0.765353, 0.744828, 0.724302, 0.711989, 0.695914, 0.704155, 0.693387, 0.676819,
                                         0.676799, 0.668149, 0.678667, 0.668981, 0.662106, 0.648292, 0.646541, 0.650966,
                                         0.634939, 0.629944, 0.638864, 0.624766, 0.619045, 0.609469, 0.610016, 0.604568,
                                         0.602381, 0.60177, 0.594209, 0.586193, 0.587409, 0.588692, 0.586485, 0.58167,
                                         0.573177, 0.572706, 0.565486, 0.569903, 0.565703, 0.562678, 0.561827, 0.555701,
                                         0.5488, 0.54492, 0.552272, 0.5472, 0.553907, 0.538975, 0.532071, 0.512574,
                                         0.501337, 0.486138, 0.474234, 0.465142, 0.467719, 0.46093, 0.464721, 0.453282,
                                         0.451003, 0.44518, 0.440071, 0.442123, 0.441155, 0.437047, 0.435727, 0.432586,
                                         0.425206, 0.427655, 0.422533, 0.424865, 0.422092, 0.420273, 0.415346, 0.409133,
                                         0.414508, 0.410388, 0.404746, 0.403885, 0.398622, 0.398415, 0.398836, 0.401538,
                                         0.403333, 0.393603, 0.391392, 0.395468, 0.388086, 0.381737, 0.38563, 0.38291,
                                         0.385134, 0.373556, 0.368796, 0.365477, 0.361356, 0.356366, 0.337974, 0.327807,
                                         0.321853, 0.316427, 0.311002 };

    const G4double EBC_BC_TilePMT[size] = { 0.7648, 0.737577, 0.710355, 0.692604, 0.686204, 0.679879, 0.671754, 0.665405,
                                          0.661076, 0.659704, 0.658373, 0.650191, 0.643016, 0.636251, 0.636981,
                                          0.631083, 0.629664, 0.624073, 0.611652, 0.616094, 0.610218, 0.605689,
                                          0.599631, 0.598852, 0.58564, 0.582424, 0.587247, 0.579566, 0.573991, 0.574983,
                                          0.569965, 0.57214, 0.565687, 0.565212, 0.553511, 0.556306, 0.55677, 0.550087,
                                          0.548716, 0.543633, 0.543634, 0.540209, 0.538204, 0.541047, 0.539491,
                                          0.531351, 0.52245, 0.512828, 0.504501, 0.491639, 0.484139, 0.475865, 0.47199,
                                          0.471604, 0.469904, 0.467587, 0.4615, 0.457367, 0.45596, 0.4502, 0.44924,
                                          0.44714, 0.448449, 0.44337, 0.436506, 0.441099, 0.433849, 0.437988, 0.432466,
                                          0.429481, 0.425351, 0.424773, 0.424019, 0.41792, 0.414599, 0.41823, 0.415068,
                                          0.4173, 0.412434, 0.413478, 0.403198, 0.409181, 0.40829, 0.404226, 0.402178,
                                          0.394833, 0.393679, 0.393674, 0.391172, 0.387596, 0.379518, 0.376184,
                                          0.373049, 0.365151, 0.355832, 0.343082, 0.33591, 0.325943, 0.315977 };

    const G4double EBC_D_TilePMT[size] = { 0.810214, 0.783731, 0.757247, 0.765208, 0.759164, 0.741329, 0.728715, 0.705468,
                                         0.696755, 0.676816, 0.667219, 0.654211, 0.652711, 0.65308, 0.645492, 0.648598,
                                         0.639812, 0.639477, 0.635602, 0.62643, 0.610883, 0.617949, 0.607085, 0.602215,
                                         0.598796, 0.598681, 0.591363, 0.587693, 0.580236, 0.579129, 0.578721, 0.569181,
                                         0.571318, 0.55844, 0.555816, 0.557064, 0.547434, 0.546389, 0.543078, 0.539943,
                                         0.53965, 0.533382, 0.530646, 0.531389, 0.531807, 0.523653, 0.513609, 0.514248,
                                         0.501599, 0.491929, 0.483775, 0.481274, 0.4627, 0.464578, 0.465772, 0.460731,
                                         0.453232, 0.448857, 0.44975, 0.444581, 0.441382, 0.437043, 0.436079, 0.437289,
                                         0.431411, 0.425935, 0.430075, 0.422739, 0.422647, 0.415665, 0.412222, 0.411782,
                                         0.409388, 0.410704, 0.405253, 0.402566, 0.398764, 0.402502, 0.392514, 0.396349,
                                         0.396642, 0.391042, 0.389076, 0.387543, 0.377968, 0.379668, 0.375101, 0.366595,
                                         0.367618, 0.360109, 0.359919, 0.350608, 0.353021, 0.348171, 0.345853, 0.337198,
                                         0.319714, 0.311871, 0.304028 };
  
    //Array of tiles distances from center in the ATLAS Experiment (from athena). 
    //In the simulation the beam position should be 2298 0 0 mm to get the same distances
    //
    const double R[11] = { 2350., 2450., 2550., 2680., 2810., 2940., 3090., 3240., 3390., 3580., 3770. };

    const double sizerange = (double) size / range;
    const double size2 = (double) size / 2.;

    if (row < 0 || row >= 11) {
        G4cout<<"-->ERROR in tile row"<<G4endl;
        return 0.;
    }
    const double phi = atan(x / (y + R[row]));
    const int index = int(phi * sizerange + size2);

    if (index < 0 || index >= size) {
        G4cout<<"-->ERROR in U-shape index"<<G4endl;
        return 0.;
    }

    auto throwCellLogicError = [&cell]() -> G4double {
        G4ExceptionDescription msg;
        msg << "Given cell does not make sense:\n"
            << cell << G4endl;
        G4Exception("ATLTileCalTBSensDet::Tile_1D_profileRescaled",
        "MyCode0006", FatalException, msg);
        return 0.;
    };

    G4double amplitude = 0.0;

    switch (cell.module) {
        case ATLTileCalTBGeometry::Module::LONG_LOWER:
        case ATLTileCalTBGeometry::Module::LONG_UPPER:
            switch (cell.row) {
                case ATLTileCalTBGeometry::Row::A:
                    amplitude = LB_A_TilePMT[index];
                    break;
                case ATLTileCalTBGeometry::Row::BC:
                    amplitude = LB_BC_TilePMT[index];
                    break;
                case ATLTileCalTBGeometry::Row::D:
                    amplitude = LB_D_TilePMT[index];
                    break;
                default:
                    return throwCellLogicError();
            }
            break;
        case ATLTileCalTBGeometry::Module::EXTENDED:
        case ATLTileCalTBGeometry::Module::EXTENDED_C10:
        case ATLTileCalTBGeometry::Module::EXTENDED_D4:
            /*
            //if (nSide > 0) {     //at TB EBC was used
            if (row < 3) {         //A layer
                amplitude = EBA_A_TilePMT[index];
            }
            else if (row < 7) {    //BC layer
                amplitude = EBA_BC_TilePMT[index];
            }
            else {                 //D layer
                amplitude = EBA_D_TilePMT[index];
            }
            //}
            */
            switch (cell.row) {
                case ATLTileCalTBGeometry::Row::A:
                    amplitude = EBC_A_TilePMT[index];
                    break;
                case ATLTileCalTBGeometry::Row::B:
                case ATLTileCalTBGeometry::Row::C:
                    amplitude = EBC_BC_TilePMT[index];
                    break;
                case ATLTileCalTBGeometry::Row::D:
                    amplitude = EBC_D_TilePMT[index];
                    break;
                default:
                    return throwCellLogicError();
            }
            break;
    }

  //G4cout<<amplitude<<G4endl;
  return amplitude;

}

//**************************************************
