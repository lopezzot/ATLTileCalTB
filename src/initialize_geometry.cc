#include "ATLTileCalTBDetConstruction.hh"

#include "G4SystemOfUnits.hh"

void ATLTileCalTBDetConstruction::initialize_geometry_description()
{
    tilegeo=new TileGeometry("TileCal Module0 Geometry Description");
    tilegeo->version(1);
    tilegeo->rmin(229.0*cm);
    tilegeo->rmax(423.0*cm);
    tilegeo->zmax(568.0*cm);
	
    TileSection* tilebarrel=new TileSection("TileCal barrel Section"); //barrel
    tilebarrel->nmodules(64);
    tilebarrel->nperiods(307);
    tilebarrel->rmin(230.0*cm);
    tilebarrel->rmax(386.0*cm);
    tilebarrel->periodthickness(1.82575*cm);
    tilebarrel->drfront(1.0*cm);
    // tilebarrel->dzend(2.0*cm);
    tilebarrel->dzend(1.75*cm); // add symmetrically 2.5mm of iron to sensitive volume
                                // because the last period has no 5mm master 
    tilebarrel->flangex(404.25*cm);
    tilebarrel->flangey(17.9*cm);
    tilebarrel->phigap(0.1*cm);
    tilebarrel->offset(0.0*cm);
	
    TileSection* tileextbarrel=new TileSection("TileCal ext. barrel Section"); //ext barrel
    tileextbarrel->nmodules(64);
    tileextbarrel->nperiods(140);
    tileextbarrel->rmin(230.0*cm);
    tileextbarrel->rmax(386.0*cm);
    tileextbarrel->periodthickness(1.82575*cm);
    tileextbarrel->drfront(1.0*cm);
    tileextbarrel->dzend(1.5*cm);
    tileextbarrel->flangex(404.25*cm);
    tileextbarrel->flangey(17.9*cm);
    tileextbarrel->phigap(0.1*cm);
    tileextbarrel->offset(482.*cm);
	
    TileSection* tileplug1=new TileSection("TileCal Plug 1 Section"); //plug 1
    tileplug1->nmodules(64);
    tileplug1->nperiods(17);
    tileplug1->rmin(344.0*cm);
    tileplug1->rmax(386.0*cm);
    tileplug1->periodthickness(1.82575*cm);
    tileplug1->drfront(.1*cm);
    tileplug1->dzend(0.05*cm);
    tileplug1->flangex(0.0*cm);
    tileplug1->flangey(0.0*cm);
    tileplug1->phigap(0.1*cm);
    tileplug1->offset(337.35*cm);


    tilegeo->section(tilebarrel);
    tilegeo->section(tileextbarrel);
    tilegeo->section(tileplug1);
	
    TileGirder* tilegirder=new TileGirder("TileCal Module0 Girder Structure");
    tilegirder->nelements(6);
    double rpos[]={ 390.125*cm,394.875*cm,413.625*cm, 404.25*cm,404.25*cm,418.625*cm};
    double delr[]={    8.25*cm,   1.25*cm,   1.25*cm,   17.5*cm,  17.5*cm,   8.75*cm};
    double delw[]={   36.40*cm,  26.00*cm,  26.00*cm,   4.25*cm,  4.25*cm,   0.00*cm};
    double offs[]={    0.00*cm,   0.00*cm,   0.00*cm,-10.875*cm,10.875*cm,   0.00*cm};
    tilegirder->rposition(6,rpos);
    tilegirder->deltar(6,delr);
    tilegirder->width(6,delw);
    tilegirder->offset(6,offs);
        
    tilebarrel->tilegirder(tilegirder);
    tileextbarrel->tilegirder(tilegirder);
    tileplug1->tilegirder(tilegirder);
    
    TileScintillator* tilescin=new TileScintillator("TileCal Module0 Scintillator");
        
    tilescin->nscintillators(11);
    double dr[]={ 9.7*cm,  9.7*cm,  9.7*cm, 12.7*cm, 12.7*cm, 12.7*cm, 14.7*cm,
                 14.7*cm, 14.7*cm, 18.7*cm, 18.7*cm};
    double rc[]={5.05*cm, 15.0*cm, 25.0*cm, 36.5*cm, 49.5*cm, 62.5*cm, 76.5*cm,
                 91.5*cm,106.5*cm,123.5*cm,142.5*cm};
//    double zp[]={-0.35,  0.15, -0.35,  0.15, -0.35,  0.15, -0.35,
//                  0.15, -0.35, 0.15,  -0.35};
    double zp[]={-0.25,  0.25, -0.25,  0.25, -0.25,  0.25, -0.25,
                  0.25, -0.25, 0.25,  -0.25};
    // Scintillator wrapping parameters
    double Dtw = 0.05*cm;
    double Drw = 0.05*cm;
    //
    tilescin->thickness(.30*cm);
    tilescin->heigth(11,dr);
    tilescin->rposition(11,rc);
    tilescin->zposition(11,zp);
    tilescin->thickwrapp(Dtw);
    tilescin->rwrapp(Drw);
        
    TileScintillator* tilepscin=new TileScintillator("TileCal plug Scintillator");
        
    tilepscin->nscintillators(2);
    double pdr[]={18.7*cm, 18.7*cm };
    double prc[]={9.5*cm, 28.5*cm };
    double pzp[]={0.0*cm,  -0.5*cm };
    tilepscin->thickness(.30*cm);
    tilepscin->heigth(2,pdr);
    tilepscin->rposition(2,prc);
    tilepscin->zposition(2,pzp);
    tilepscin->thickwrapp(Dtw);
    tilepscin->rwrapp(Drw);
        
    tilebarrel->tilescintillator(tilescin);
    tileextbarrel->tilescintillator(tilescin);
    tileplug1->tilescintillator(tilepscin);

    TileCell* barrelcell=new TileCell("TileCal Barrel Cell");
        
    int sample[]={ 0,0,0, 1,1,1,1,1,1, 2,2};
    barrelcell->scin2sample(11,sample);

    int r0[] = { 16, 18, 18, 16, 15, 15, 14, 14, 14, 13, 14, 13, 14, 14, 15, 16, 16, 17, 19, 16 };
    int r1[] = { 16, 19, 17, 16, 16, 15, 14, 14, 13, 14, 13, 14, 14, 14, 15, 15, 16, 18, 18, 16 };
    int r2[] = { 16, 18, 18, 16, 15, 15, 14, 14, 14, 13, 14, 13, 14, 14, 15, 16, 16, 17, 19, 16 };
    int r3[] = { 18, 20, 18, 18, 17, 16, 16, 15, 16, 15, 16, 15, 17, 16, 18, 19, 20, 17 };
    int r4[] = { 17, 20, 19, 18, 16, 17, 15, 16, 15, 16, 15, 16, 16, 17, 18, 18, 20, 18 };
    int r5[] = { 18, 20, 18, 18, 17, 16, 16, 15, 16, 15, 16, 15, 17, 16, 18, 19, 20, 17 };
    int r6[] = {  0, 20, 21, 21, 19, 19, 18, 18, 17, 18, 18, 18, 19, 19, 20, 22, 20,  0 };
    int r7[] = {  0, 20, 22, 20, 19, 19, 18, 18, 18, 17, 18, 18, 19, 19, 21, 21, 20,  0 };
    int r8[] = {  0, 20, 21, 21, 19, 19, 18, 18, 17, 18, 18, 18, 19, 19, 20, 22, 20,  0 };
    int r9[] = { 50, 43, 41, 40, 40, 43, 50 };
    int r10[]= { 50, 43, 40, 40, 41, 43, 50 };
    int *row[11] = {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10};
    barrelcell->scin2cell(11,307,row);
/*        
    for (int sci=0; sci<11; ++sci) {
        printf("Row %2d - sample %d\n",sci,barrelcell->scin2sample(sci));
        for (int per=0; per<307; ++per) {
            printf(" %d-%d",per,barrelcell->scin2cell(per,sci));
        }
        printf("\n===========================\n");
    }
*/        
    tilebarrel->tilecell(barrelcell);
    tileextbarrel->tilecell(barrelcell); // FIXME: another cell needed
    tileplug1->tilecell(barrelcell);     // FIXME: another cell needed
}
