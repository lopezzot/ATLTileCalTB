//**************************************************
// \file ATLTileCalTBConstants.hh
// \brief: definition of ATLTileCalTBConstants
//         namespace
// \author: Stephan Lachnit (CERN EP-SFT-sim) 
//          @stephanlachnit
// \start date: 7 July 2022
//**************************************************


#ifndef ATLTileCalTBConstants_h
#define ATLTileCalTBConstants_h 1

//Includers from Geant4
//
#include "globals.hh"
#include "G4SystemOfUnits.hh"


namespace ATLTileCalTBConstants {

    // Amount of energy deposited in the tiles compared to the total energy in the calorimeter
    constexpr G4double sampling_fraction = 3.5 * perCent;

    // Amount of photoelectrons created per energy (adjusted by the sampling fraction)
    constexpr G4double photoelectrons_per_energy = 70. / GeV / sampling_fraction;

    // Sigma of the electronic noise (white noise / gaussian)
    constexpr G4double noise_sigma = (30 * MeV) * photoelectrons_per_energy;

    // Digitization: bin width of early hit frames
    constexpr G4double early_frame_bin_time = 0.5 * ns;

    // Digitization: time window where hit frames are marked as early
    constexpr G4double early_frame_time_window = 150 * ns;

    // Digitization: amount of early time frames
    constexpr std::size_t early_frames = static_cast<std::size_t>(early_frame_time_window / early_frame_bin_time);

    // Digitization: bin width of late hit frames
    constexpr G4double late_frame_bin_time = 5 * ns;

    // Digitization: time window where hit frames are marked as late
    constexpr G4double late_frame_time_window = 200 * ns;

    // Digitization: amount of late time frames
    constexpr std::size_t late_frames = static_cast<std::size_t>(late_frame_time_window / late_frame_bin_time);

    // Digitization: total time window
    constexpr G4double total_time_window = early_frame_time_window + late_frame_time_window;

    // Digitization: total amount of time frames
    constexpr std::size_t total_frames = early_frames + late_frames;

    // Digitization: analog response of the PMT to one photoelectron (0.5ns bins)
    // From https://gitlab.cern.ch/atlas/athena/-/blob/1a58a6b7cc3d6e02c664814502796aa9f86eab7c/TileCalorimeter/TileConditions/share/pulsehi_physics.dat
    constexpr std::array<G4double, 401> pmt_response = {
        0.00000000,
        0.00002304,
        0.00005178,
        0.00008592,
        0.00012515,
        0.00016918,
        0.00021770,
        0.00027043,
        0.00032705,
        0.00038728,
        0.00045081,
        0.00051733,
        0.00058656,
        0.00065819,
        0.00073193,
        0.00080746,
        0.00088450,
        0.00096275,
        0.00104190,
        0.00112166,
        0.00120172,
        0.00128179,
        0.00136157,
        0.00144076,
        0.00151905,
        0.00159642,
        0.00167540,
        0.00176001,
        0.00185428,
        0.00196225,
        0.00208795,
        0.00223542,
        0.00240868,
        0.00261177,
        0.00284873,
        0.00312358,
        0.00344037,
        0.00380312,
        0.00421586,
        0.00468264,
        0.00520748,
        0.00579479,
        0.00645263,
        0.00719117,
        0.00802059,
        0.00895106,
        0.00999278,
        0.01115590,
        0.01245070,
        0.01388720,
        0.01547570,
        0.01722640,
        0.01914940,
        0.02125490,
        0.02355310,
        0.02605410,
        0.02876670,
        0.03169910,
        0.03485930,
        0.03825560,
        0.04189600,
        0.04578870,
        0.04994190,
        0.05436370,
        0.05906210,
        0.06404550,
        0.06932180,
        0.07489930,
        0.08078610,
        0.08699030,
        0.09352000,
        0.10038300,
        0.10758100,
        0.11511500,
        0.12298300,
        0.13118400,
        0.13971900,
        0.14858600,
        0.15778600,
        0.16731600,
        0.17717700,
        0.18736900,
        0.19789000,
        0.20873900,
        0.21991700,
        0.23142300,
        0.24324800,
        0.25538500,
        0.26782200,
        0.28055000,
        0.29356000,
        0.30684200,
        0.32038500,
        0.33418100,
        0.34821900,
        0.36248900,
        0.37698200,
        0.39168800,
        0.40659700,
        0.42169900,
        0.43698600,
        0.45244500,
        0.46806000,
        0.48380800,
        0.49966800,
        0.51561700,
        0.53163500,
        0.54769800,
        0.56378600,
        0.57987600,
        0.59594600,
        0.61197500,
        0.62794000,
        0.64382000,
        0.65959200,
        0.67523600,
        0.69073000,
        0.70605300,
        0.72118600,
        0.73610900,
        0.75080000,
        0.76524000,
        0.77940800,
        0.79328500,
        0.80684900,
        0.82008000,
        0.83295900,
        0.84546400,
        0.85757600,
        0.86927400,
        0.88053800,
        0.89134900,
        0.90169300,
        0.91156400,
        0.92095400,
        0.92985400,
        0.93825700,
        0.94615600,
        0.95354100,
        0.96040600,
        0.96674200,
        0.97254200,
        0.97779800,
        0.98250100,
        0.98664500,
        0.99022200,
        0.99323400,
        0.99568500,
        0.99758100,
        0.99892900,
        0.99973300,
        1.00000000,
        0.99973500,
        0.99894400,
        0.99763200,
        0.99580600,
        0.99347000,
        0.99063000,
        0.98729300,
        0.98346400,
        0.97914800,
        0.97435200,
        0.96908900,
        0.96337600,
        0.95723100,
        0.95067000,
        0.94371100,
        0.93637100,
        0.92866600,
        0.92061500,
        0.91223500,
        0.90354200,
        0.89455300,
        0.88528700,
        0.87575900,
        0.86598800,
        0.85598500,
        0.84576300,
        0.83533200,
        0.82470400,
        0.81389000,
        0.80290000,
        0.79174600,
        0.78043900,
        0.76899100,
        0.75741200,
        0.74571300,
        0.73390600,
        0.72200200,
        0.71001200,
        0.69794700,
        0.68581800,
        0.67363500,
        0.66141000,
        0.64915300,
        0.63687300,
        0.62458100,
        0.61228800,
        0.60000400,
        0.58773900,
        0.57550300,
        0.56330700,
        0.55116100,
        0.53907600,
        0.52706200,
        0.51512900,
        0.50328600,
        0.49154200,
        0.47990600,
        0.46838600,
        0.45699100,
        0.44572900,
        0.43460900,
        0.42364000,
        0.41283100,
        0.40218900,
        0.39172400,
        0.38144400,
        0.37135800,
        0.36147400,
        0.35180200,
        0.34234800,
        0.33311500,
        0.32410000,
        0.31530100,
        0.30671500,
        0.29833900,
        0.29017200,
        0.28221100,
        0.27445300,
        0.26689700,
        0.25953900,
        0.25237700,
        0.24540900,
        0.23863200,
        0.23204400,
        0.22564200,
        0.21942100,
        0.21337900,
        0.20751100,
        0.20181300,
        0.19628300,
        0.19091500,
        0.18570700,
        0.18065500,
        0.17575500,
        0.17100300,
        0.16639500,
        0.16192800,
        0.15759800,
        0.15340200,
        0.14933500,
        0.14539400,
        0.14157500,
        0.13787400,
        0.13428800,
        0.13081300,
        0.12744500,
        0.12418000,
        0.12101500,
        0.11794700,
        0.11497000,
        0.11208200,
        0.10927800,
        0.10655600,
        0.10391100,
        0.10134000,
        0.09884270,
        0.09641560,
        0.09405690,
        0.09176460,
        0.08953670,
        0.08737090,
        0.08526520,
        0.08321760,
        0.08122590,
        0.07928810,
        0.07740210,
        0.07556570,
        0.07377690,
        0.07203370,
        0.07033380,
        0.06867530,
        0.06705610,
        0.06547400,
        0.06392690,
        0.06241290,
        0.06092980,
        0.05947550,
        0.05804790,
        0.05664490,
        0.05526450,
        0.05390460,
        0.05256300,
        0.05123770,
        0.04992680,
        0.04862960,
        0.04734660,
        0.04607780,
        0.04482350,
        0.04358410,
        0.04235980,
        0.04115080,
        0.03995730,
        0.03877980,
        0.03761830,
        0.03647320,
        0.03534470,
        0.03423310,
        0.03313860,
        0.03206150,
        0.03100210,
        0.02996050,
        0.02893720,
        0.02793220,
        0.02694590,
        0.02597860,
        0.02503050,
        0.02410180,
        0.02319280,
        0.02230380,
        0.02143500,
        0.02058670,
        0.01975910,
        0.01895250,
        0.01816710,
        0.01740300,
        0.01666000,
        0.01593790,
        0.01523670,
        0.01455620,
        0.01389620,
        0.01325670,
        0.01263740,
        0.01203830,
        0.01145920,
        0.01090000,
        0.01036060,
        0.00984081,
        0.00934049,
        0.00885952,
        0.00839776,
        0.00795509,
        0.00753137,
        0.00712647,
        0.00674024,
        0.00637256,
        0.00602330,
        0.00569231,
        0.00537947,
        0.00508463,
        0.00480767,
        0.00454845,
        0.00430684,
        0.00408270,
        0.00387587,
        0.00368583,
        0.00351191,
        0.00335339,
        0.00320960,
        0.00307982,
        0.00296336,
        0.00285953,
        0.00276764,
        0.00268698,
        0.00261686,
        0.00255658,
        0.00250545,
        0.00246277,
        0.00242785,
        0.00239998,
        0.00237849,
        0.00236265,
        0.00235179,
        0.00234521,
        0.00234220,
        0.00234208,
        0.00234415,
        0.00234770,
        0.00235206,
        0.00235651,
        0.00236036,
        0.00236293,
        0.00236350,
        0.00236139,
        0.00235594,
        0.00234693,
        0.00233436,
        0.00231825,
        0.00229862,
        0.00227549,
        0.00224886,
        0.00221876,
        0.00218520,
        0.00214819,
        0.00210775,
        0.00206390,
        0.00201666,
        0.00196603,
        0.00191204,
        0.00185470,
    };

};

#endif //ATLTileCalTBConstants_h

//**************************************************
