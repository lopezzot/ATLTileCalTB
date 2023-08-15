//**************************************************
// \file FLUKA_comparison.C
// \brief: G4-to-Fluka comparison plots
// \author: Michele D'Andrea (CERN EP-SFT-sim)
//          @DandreaMichele
// \start date: 12 August 2023
//**************************************************

#ifndef FLUKA_comparison_h
#define FLUKA_comparison_h

#include "TBrun_all.C"

void FLUKA_comparison(){

    //Running single analysis
    cout << "Starting G4 standalone analysis...\n" << endl;
    TBrun_all();
    cout << "Starting FLUKA analysis...\n" << endl;
    TBrun_all(true);

    //defining variables
    Double_t xprime, yprime;
    double xArray[4];
    double ATLAS_energyresolution[4];
    double ATLAS_energyresolution_error[4];
    double FLUKA_energyresolution[4];
    double FLUKA_energyresolution_error[4];
    double G4_energyresolution[4];
    double G4_energyresolution_error[4];
    double ATLAS_energyresponse[4];
    double ATLAS_energyresponse_error[4];
    double FLUKA_energyresponse[4];
    double FLUKA_energyresponse_error[4];
    double G4_energyresponse[4];
    double G4_energyresponse_error[4];

    //Initialize the plots
    cout << "Initializing plots for comparison..." << endl;

    auto outputfile = new TFile("FLUKA_comparison.root","RECREATE");
    outputfile->cd();

    //Energy resolution plot------------------------------------------
    //
    //#######---------Importing data from files-----------#######
    auto* G4_file = new TFile("analysis.root");
    auto* FLUKA_file = new TFile("analysis_fluka.root");
    
    //-------------------------Pions--------------------------------

    //------ATLAS---------
    auto ATLAS_EnergyRes = (TGraphErrors*)G4_file->Get("ATLAS_Energy_Resolution_Pions");
    for (unsigned int i=0; i<4; i++){
        ATLAS_EnergyRes->GetPoint(i, xprime, yprime);
        ATLAS_energyresolution[i] = yprime;
        ATLAS_energyresolution_error[i] = ATLAS_EnergyRes->GetErrorY(i);
    }
    //ATLAS_EnergyRes->SetMarkerStyle(8);
    ATLAS_EnergyRes -> SetLineColor(kBlue);
    ATLAS_EnergyRes -> SetMarkerColor(kBlue);
    ATLAS_EnergyRes -> SetMarkerStyle(kFullCircle);
    ATLAS_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);

    //------FLUKA & RATIO---------
    auto FLUKA_EnergyRes = (TGraphErrors*)FLUKA_file->Get("Energy_Resolution_Pions");
    FLUKA_EnergyRes -> SetMarkerStyle(kCircle);
    FLUKA_EnergyRes -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyRes -> SetLineColor(kCyan -3);
    FLUKA_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);
    
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyRes->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        FLUKA_energyresolution[i] = yprime;
        FLUKA_energyresolution_error[i] = FLUKA_EnergyRes->GetErrorY(i);
    }
    
    outputfile->cd();
    TGraphErrors* FLUKA_EnergyRes_ratio = new TGraphErrors(4);FLUKA_EnergyRes_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyRes_ratio->SetPoint(i, xArray[i], FLUKA_energyresolution[i]/ATLAS_energyresolution[i]);
        FLUKA_EnergyRes_ratio->SetPointError(i, 0.,
                (FLUKA_energyresolution_error[i]/FLUKA_energyresolution[i]+
                 ATLAS_energyresolution_error[i]/ATLAS_energyresolution[i])*FLUKA_energyresolution[i]/ATLAS_energyresolution[i]);
    }
    FLUKA_EnergyRes_ratio->GetYaxis()->SetTitle("MC/Data");
    FLUKA_EnergyRes_ratio->SetMarkerStyle(kCircle);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetLabelSize(0.09);FLUKA_EnergyRes_ratio->GetXaxis()->SetLabelSize(0.09);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetTitleSize(0.09);FLUKA_EnergyRes_ratio->GetYaxis()->SetTitleOffset(0.65);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetRangeUser(0.7,1.05);
    FLUKA_EnergyRes_ratio->SetName("Pions_ATLASenergyresolutionRatio");
    FLUKA_EnergyRes_ratio->Draw("AP");
    FLUKA_EnergyRes_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyRes_ratio -> SetLineColor(kCyan -3);
    FLUKA_EnergyRes_ratio->Write();
    
    //------G4StandAlone & RATIO---------
    auto G4_EnergyRes = (TGraphErrors*)G4_file->Get("Energy_Resolution_Pions");
    G4_EnergyRes -> SetMarkerStyle(kCircle);
    G4_EnergyRes -> SetMarkerColor(kRed);
    G4_EnergyRes -> SetLineColor(kRed);
    G4_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);

    for (unsigned int i=0; i<4; i++){
        G4_EnergyRes->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        G4_energyresolution[i] = yprime;
        G4_energyresolution_error[i] = G4_EnergyRes->GetErrorY(i);
    }

    outputfile->cd();

    TGraphErrors* G4_EnergyRes_ratio = new TGraphErrors(4);G4_EnergyRes_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        G4_EnergyRes_ratio->SetPoint(i, xArray[i], G4_energyresolution[i]/ATLAS_energyresolution[i]);
        G4_EnergyRes_ratio->SetPointError(i, 0.,
                (G4_energyresolution_error[i]/G4_energyresolution[i]+
                 ATLAS_energyresolution_error[i]/ATLAS_energyresolution[i])*G4_energyresolution[i]/ATLAS_energyresolution[i]);
    }
    G4_EnergyRes_ratio->GetYaxis()->SetTitle("MC/Data");
    G4_EnergyRes_ratio->SetMarkerStyle(kCircle);
    G4_EnergyRes_ratio->GetYaxis()->SetLabelSize(0.09);G4_EnergyRes_ratio->GetXaxis()->SetLabelSize(0.09);
    G4_EnergyRes_ratio->GetYaxis()->SetTitleSize(0.09);G4_EnergyRes_ratio->GetYaxis()->SetTitleOffset(0.65);
    G4_EnergyRes_ratio->GetYaxis()->SetRangeUser(0.7,1.05);
    G4_EnergyRes_ratio->SetName("Pions_ATLASenergyresolutionRatio");
    G4_EnergyRes_ratio->Draw("AP");
    G4_EnergyRes_ratio -> SetMarkerColor(kRed);
    G4_EnergyRes_ratio -> SetLineColor(kRed);
    G4_EnergyRes_ratio->Write();

    outputfile->cd();

    //#######---------Plotting-----------#######

    auto C1res = new TCanvas("ComparisonEnergyResolution", "", 700, 900);
    auto *p1res = new TPad("p1","p1",0.,0.305,1.,1.);  p1res->Draw();
    auto *p2res = new TPad("p2","p2",0.,0.02,1.,0.32); p2res->Draw();
    
    p1res->cd();
    ATLAS_EnergyRes->SetTitle("");
    ATLAS_EnergyRes->Draw("AP");
    G4_EnergyRes->Draw("same P");
    FLUKA_EnergyRes->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto* Freslegend = new TLegend(0.22,0.6,1.-0.51,0.89);
    Freslegend->SetHeader("PIONS");
    Freslegend->AddEntry(ATLAS_EnergyRes,
    "Experimental data", "P");
    Freslegend->AddEntry(G4_EnergyRes,
    "GEANT4", "P");
    Freslegend->AddEntry(FLUKA_EnergyRes,
    "GH", "P");
    Freslegend->SetLineWidth(0);
    Freslegend->Draw("same");
    p1res->SetTitle("Pions Energy Resolution");
    
    p2res->cd();
    gPad->SetLeftMargin(0.15);
    G4_EnergyRes_ratio->Draw("AP");
    FLUKA_EnergyRes_ratio->Draw("same P");

    C1res->Write();
    //C1res->SaveAs("plots/PIONS_EnergyRes_Comparison.pdf");
    delete p1res, p2res;
    delete G4_EnergyRes, ATLAS_EnergyRes, FLUKA_EnergyRes, G4_EnergyRes_ratio, FLUKA_EnergyRes_ratio;
    delete C1res;

    //-----------------KAONS-------------------------------------------------------

    //------ATLAS---------
    ATLAS_EnergyRes = (TGraphErrors*)G4_file->Get("ATLAS_Energy_Resolution_Kaons");
    for (unsigned int i=0; i<4; i++){
        ATLAS_EnergyRes->GetPoint(i, xprime, yprime);
        ATLAS_energyresolution[i] = yprime;
        ATLAS_energyresolution_error[i] = ATLAS_EnergyRes->GetErrorY(i);
    }
    //ATLAS_EnergyRes->SetMarkerStyle(8);
    ATLAS_EnergyRes -> SetLineColor(kBlue);
    ATLAS_EnergyRes -> SetMarkerColor(kBlue);
    ATLAS_EnergyRes -> SetMarkerStyle(kFullCircle);
    ATLAS_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);

    //------FLUKA & RATIO---------
    FLUKA_EnergyRes = (TGraphErrors*)FLUKA_file->Get("Energy_Resolution_Kaons");
    FLUKA_EnergyRes -> SetMarkerStyle(kCircle);
    FLUKA_EnergyRes -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyRes -> SetLineColor(kCyan -3);
    FLUKA_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);
    
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyRes->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        FLUKA_energyresolution[i] = yprime;
        FLUKA_energyresolution_error[i] = FLUKA_EnergyRes->GetErrorY(i);
    }
    
    outputfile->cd();
    FLUKA_EnergyRes_ratio = new TGraphErrors(4);FLUKA_EnergyRes_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyRes_ratio->SetPoint(i, xArray[i], FLUKA_energyresolution[i]/ATLAS_energyresolution[i]);
        FLUKA_EnergyRes_ratio->SetPointError(i, 0.,
                (FLUKA_energyresolution_error[i]/FLUKA_energyresolution[i]+
                 ATLAS_energyresolution_error[i]/ATLAS_energyresolution[i])*FLUKA_energyresolution[i]/ATLAS_energyresolution[i]);
    }
    FLUKA_EnergyRes_ratio->GetYaxis()->SetTitle("MC/Data");
    FLUKA_EnergyRes_ratio->SetMarkerStyle(kCircle);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetLabelSize(0.09);FLUKA_EnergyRes_ratio->GetXaxis()->SetLabelSize(0.09);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetTitleSize(0.09);FLUKA_EnergyRes_ratio->GetYaxis()->SetTitleOffset(0.65);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetRangeUser(0.7,1.05);
    FLUKA_EnergyRes_ratio->SetName("Kaons_ATLASenergyresolutionRatio");
    FLUKA_EnergyRes_ratio->Draw("AP");
    FLUKA_EnergyRes_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyRes_ratio -> SetLineColor(kCyan -3);
    FLUKA_EnergyRes_ratio->Write();
    
    //------G4StandAlone & RATIO---------
    G4_EnergyRes = (TGraphErrors*)G4_file->Get("Energy_Resolution_Kaons");
    G4_EnergyRes -> SetMarkerStyle(kCircle);
    G4_EnergyRes -> SetMarkerColor(kRed);
    G4_EnergyRes -> SetLineColor(kRed);
    G4_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);

    for (unsigned int i=0; i<4; i++){
        G4_EnergyRes->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        G4_energyresolution[i] = yprime;
        G4_energyresolution_error[i] = G4_EnergyRes->GetErrorY(i);
    }

    outputfile->cd();

    G4_EnergyRes_ratio = new TGraphErrors(4);G4_EnergyRes_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        G4_EnergyRes_ratio->SetPoint(i, xArray[i], G4_energyresolution[i]/ATLAS_energyresolution[i]);
        G4_EnergyRes_ratio->SetPointError(i, 0.,
                (G4_energyresolution_error[i]/G4_energyresolution[i]+
                 ATLAS_energyresolution_error[i]/ATLAS_energyresolution[i])*G4_energyresolution[i]/ATLAS_energyresolution[i]);
    }
    G4_EnergyRes_ratio->GetYaxis()->SetTitle("MC/Data");
    G4_EnergyRes_ratio->SetMarkerStyle(kCircle);
    G4_EnergyRes_ratio->GetYaxis()->SetLabelSize(0.09);G4_EnergyRes_ratio->GetXaxis()->SetLabelSize(0.09);
    G4_EnergyRes_ratio->GetYaxis()->SetTitleSize(0.09);G4_EnergyRes_ratio->GetYaxis()->SetTitleOffset(0.65);
    G4_EnergyRes_ratio->GetYaxis()->SetRangeUser(0.7,1.05);
    G4_EnergyRes_ratio->SetName("Kaons_ATLASenergyresolutionRatio");
    G4_EnergyRes_ratio->Draw("AP");
    G4_EnergyRes_ratio -> SetMarkerColor(kRed);
    G4_EnergyRes_ratio -> SetLineColor(kRed);
    G4_EnergyRes_ratio->Write();

    outputfile->cd();

    //#######---------Plotting-----------#######

    C1res = new TCanvas("ComparisonEnergyResolution", "", 700, 900);
    p1res = new TPad("p1","p1",0.,0.305,1.,1.);  p1res->Draw();
    p2res = new TPad("p2","p2",0.,0.02,1.,0.32); p2res->Draw();
    
    p1res->cd();
    //title
    p1res->SetTitle("Kaons Energy Resolution");
    ATLAS_EnergyRes->SetTitle("");
    ATLAS_EnergyRes->Draw("AP");
    G4_EnergyRes->Draw("same P");
    FLUKA_EnergyRes->Draw("same P");
    gPad->SetLeftMargin(0.15);
    Freslegend = new TLegend(0.22,0.6,1.-0.51,0.89);
    Freslegend->SetHeader("KAONS");
    Freslegend->AddEntry(ATLAS_EnergyRes,
    "Experimental data", "P");
    Freslegend->AddEntry(G4_EnergyRes,
    "GEANT4", "P");
    Freslegend->AddEntry(FLUKA_EnergyRes,
    "GH", "P");
    Freslegend->SetLineWidth(0);
    Freslegend->Draw("same");
    
    p2res->cd();
    gPad->SetLeftMargin(0.15);
    G4_EnergyRes_ratio->Draw("AP");
    FLUKA_EnergyRes_ratio->Draw("same P");

    C1res->Write();
    //C1res->SaveAs("plots/KAONS_EnergyRes_Comparison.pdf");
    delete p1res, p2res;
    delete G4_EnergyRes, ATLAS_EnergyRes, FLUKA_EnergyRes, G4_EnergyRes_ratio, FLUKA_EnergyRes_ratio;
    delete C1res;

    //-----------------PROTONS-------------------------------------------------------

    //------ATLAS---------
    ATLAS_EnergyRes = (TGraphErrors*)G4_file->Get("ATLAS_Energy_Resolution_Protons");
    for (unsigned int i=0; i<4; i++){
        ATLAS_EnergyRes->GetPoint(i, xprime, yprime);
        ATLAS_energyresolution[i] = yprime;
        ATLAS_energyresolution_error[i] = ATLAS_EnergyRes->GetErrorY(i);
    }
    //ATLAS_EnergyRes->SetMarkerStyle(8);
    ATLAS_EnergyRes -> SetLineColor(kBlue);
    ATLAS_EnergyRes -> SetMarkerColor(kBlue);
    ATLAS_EnergyRes -> SetMarkerStyle(kFullCircle);
    ATLAS_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);

    //------FLUKA & RATIO---------
    FLUKA_EnergyRes = (TGraphErrors*)FLUKA_file->Get("Energy_Resolution_Protons");
    FLUKA_EnergyRes -> SetMarkerStyle(kCircle);
    FLUKA_EnergyRes -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyRes -> SetLineColor(kCyan -3);
    FLUKA_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);
    
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyRes->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        FLUKA_energyresolution[i] = yprime;
        FLUKA_energyresolution_error[i] = FLUKA_EnergyRes->GetErrorY(i);
    }
    
    outputfile->cd();
    FLUKA_EnergyRes_ratio = new TGraphErrors(4);FLUKA_EnergyRes_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyRes_ratio->SetPoint(i, xArray[i], FLUKA_energyresolution[i]/ATLAS_energyresolution[i]);
        FLUKA_EnergyRes_ratio->SetPointError(i, 0.,
                (FLUKA_energyresolution_error[i]/FLUKA_energyresolution[i]+
                 ATLAS_energyresolution_error[i]/ATLAS_energyresolution[i])*FLUKA_energyresolution[i]/ATLAS_energyresolution[i]);
    }
    FLUKA_EnergyRes_ratio->GetYaxis()->SetTitle("MC/Data");
    FLUKA_EnergyRes_ratio->SetMarkerStyle(kCircle);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetLabelSize(0.09);FLUKA_EnergyRes_ratio->GetXaxis()->SetLabelSize(0.09);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetTitleSize(0.09);FLUKA_EnergyRes_ratio->GetYaxis()->SetTitleOffset(0.65);
    FLUKA_EnergyRes_ratio->GetYaxis()->SetRangeUser(0.7,1.05);
    FLUKA_EnergyRes_ratio->SetName("Protons_ATLASenergyresolutionRatio");
    FLUKA_EnergyRes_ratio->Draw("AP");
    FLUKA_EnergyRes_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyRes_ratio -> SetLineColor(kCyan -3);
    FLUKA_EnergyRes_ratio->Write();
    
    //------G4StandAlone & RATIO---------
    G4_EnergyRes = (TGraphErrors*)G4_file->Get("Energy_Resolution_Protons");
    G4_EnergyRes -> SetMarkerStyle(kCircle);
    G4_EnergyRes -> SetMarkerColor(kRed);
    G4_EnergyRes -> SetLineColor(kRed);
    G4_EnergyRes -> GetYaxis()->SetRangeUser(0.07, 0.14);

    for (unsigned int i=0; i<4; i++){
        G4_EnergyRes->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        G4_energyresolution[i] = yprime;
        G4_energyresolution_error[i] = G4_EnergyRes->GetErrorY(i);
    }

    outputfile->cd();

    G4_EnergyRes_ratio = new TGraphErrors(4);G4_EnergyRes_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        G4_EnergyRes_ratio->SetPoint(i, xArray[i], G4_energyresolution[i]/ATLAS_energyresolution[i]);
        G4_EnergyRes_ratio->SetPointError(i, 0.,
                (G4_energyresolution_error[i]/G4_energyresolution[i]+
                 ATLAS_energyresolution_error[i]/ATLAS_energyresolution[i])*G4_energyresolution[i]/ATLAS_energyresolution[i]);
    }
    G4_EnergyRes_ratio->GetYaxis()->SetTitle("MC/Data");
    G4_EnergyRes_ratio->SetMarkerStyle(kCircle);
    G4_EnergyRes_ratio->GetYaxis()->SetLabelSize(0.09);G4_EnergyRes_ratio->GetXaxis()->SetLabelSize(0.09);
    G4_EnergyRes_ratio->GetYaxis()->SetTitleSize(0.09);G4_EnergyRes_ratio->GetYaxis()->SetTitleOffset(0.65);
    G4_EnergyRes_ratio->GetYaxis()->SetRangeUser(0.7,1.05);
    G4_EnergyRes_ratio->SetName("Protons_ATLASenergyresolutionRatio");
    G4_EnergyRes_ratio->Draw("AP");
    G4_EnergyRes_ratio -> SetMarkerColor(kRed);
    G4_EnergyRes_ratio -> SetLineColor(kRed);
    G4_EnergyRes_ratio->Write();

    outputfile->cd();

    //#######---------Plotting-----------#######

    C1res = new TCanvas("ComparisonEnergyResolution", "", 700, 900);
    p1res = new TPad("p1","p1",0.,0.305,1.,1.);  p1res->Draw();
    p2res = new TPad("p2","p2",0.,0.02,1.,0.32); p2res->Draw();
    
    p1res->cd();
    //title
    p1res->SetTitle("Protons Energy Resolution");
    ATLAS_EnergyRes->SetTitle("");
    ATLAS_EnergyRes->Draw("AP");
    G4_EnergyRes->Draw("same P");
    FLUKA_EnergyRes->Draw("same P");
    gPad->SetLeftMargin(0.15);
    Freslegend = new TLegend(0.22,0.6,1.-0.51,0.89);
    Freslegend->SetHeader("PROTONS");
    Freslegend->AddEntry(ATLAS_EnergyRes,
    "Experimental data", "P");
    Freslegend->AddEntry(G4_EnergyRes,
    "GEANT4", "P");
    Freslegend->AddEntry(FLUKA_EnergyRes,
    "GH", "P");
    Freslegend->SetLineWidth(0);
    Freslegend->Draw("same");
    
    p2res->cd();
    gPad->SetLeftMargin(0.15);
    G4_EnergyRes_ratio->Draw("AP");
    FLUKA_EnergyRes_ratio->Draw("same P");

    C1res->Write();
    //C1res->SaveAs("plots/PROTONS_EnergyRes_Comparison.pdf");
    delete p1res, p2res;
    delete G4_EnergyRes, ATLAS_EnergyRes, FLUKA_EnergyRes, G4_EnergyRes_ratio, FLUKA_EnergyRes_ratio;
    delete C1res;

    //#######---------Energy Response-----------#######

    //-----------------PIONS-----------------

    //------ATLAS---------
    auto ATLAS_EnergyResponse = (TGraphErrors*)G4_file->Get("ATLAS_Energy_Response_Pions");
    for (unsigned int i=0; i<4; i++){
        ATLAS_EnergyResponse->GetPoint(i, xprime, yprime);
        ATLAS_energyresponse[i] = yprime;
        ATLAS_energyresponse_error[i] = ATLAS_EnergyResponse->GetErrorY(i);
    }
    //ATLAS_EnergyRes->SetMarkerStyle(8);
    ATLAS_EnergyResponse -> SetLineColor(kBlue);
    ATLAS_EnergyResponse -> SetMarkerColor(kBlue);
    ATLAS_EnergyResponse -> SetMarkerStyle(kFullCircle);
    ATLAS_EnergyResponse -> GetYaxis()->SetRangeUser(0.74, 0.85);

    //------FLUKA & RATIO---------
    auto FLUKA_EnergyResponse = (TGraphErrors*)FLUKA_file->Get("Energy_Response_Pions");
    FLUKA_EnergyResponse -> SetMarkerStyle(kCircle);
    FLUKA_EnergyResponse -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyResponse -> SetLineColor(kCyan -3);
    FLUKA_EnergyResponse -> GetYaxis()->SetRangeUser(0.74, 0.85);
    
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyResponse->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        FLUKA_energyresponse[i] = yprime;
        FLUKA_energyresponse_error[i] = FLUKA_EnergyResponse->GetErrorY(i);
    }
    
    outputfile->cd();
    TGraphErrors* FLUKA_EnergyResponse_ratio = new TGraphErrors(4);FLUKA_EnergyResponse_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyResponse_ratio->SetPoint(i, xArray[i], FLUKA_energyresponse[i]/ATLAS_energyresponse[i]);
        FLUKA_EnergyResponse_ratio->SetPointError(i, 0.,
                (FLUKA_energyresponse_error[i]/FLUKA_energyresponse[i]+
                 ATLAS_energyresponse_error[i]/ATLAS_energyresponse[i])*FLUKA_energyresponse[i]/ATLAS_energyresponse[i]);
    }
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitle("MC/Data");
    FLUKA_EnergyResponse_ratio->SetMarkerStyle(kCircle);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetLabelSize(0.09);FLUKA_EnergyResponse_ratio->GetXaxis()->SetLabelSize(0.09);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitleSize(0.09);FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitleOffset(0.65);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetRangeUser(0.95,1.05);
    FLUKA_EnergyResponse_ratio->SetName("Pions_ATLASenergyresponseRatio");
    FLUKA_EnergyResponse_ratio->Draw("AP");
    FLUKA_EnergyResponse_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyResponse_ratio -> SetLineColor(kCyan -3);
    FLUKA_EnergyResponse_ratio->Write();
    
    //------G4StandAlone & RATIO---------
    auto G4_EnergyResponse = (TGraphErrors*)G4_file->Get("Energy_Response_Pions");
    G4_EnergyResponse -> SetMarkerStyle(kCircle);
    G4_EnergyResponse -> SetMarkerColor(kRed);
    G4_EnergyResponse -> SetLineColor(kRed);
    G4_EnergyResponse -> GetYaxis()->SetRangeUser(0.74, 0.85);

    for (unsigned int i=0; i<4; i++){
        G4_EnergyResponse->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        G4_energyresponse[i] = yprime;
        G4_energyresponse_error[i] = G4_EnergyResponse->GetErrorY(i);
    }

    outputfile->cd();

    TGraphErrors* G4_EnergyResponse_ratio = new TGraphErrors(4);G4_EnergyResponse_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        G4_EnergyResponse_ratio->SetPoint(i, xArray[i], G4_energyresponse[i]/ATLAS_energyresponse[i]);
        G4_EnergyResponse_ratio->SetPointError(i, 0.,
                (G4_energyresponse_error[i]/G4_energyresponse[i]+
                 ATLAS_energyresponse_error[i]/ATLAS_energyresponse[i])*G4_energyresponse[i]/ATLAS_energyresponse[i]);
    }
    G4_EnergyResponse_ratio->GetYaxis()->SetTitle("MC/Data");
    G4_EnergyResponse_ratio->SetMarkerStyle(kCircle);
    G4_EnergyResponse_ratio->GetYaxis()->SetLabelSize(0.09);G4_EnergyResponse_ratio->GetXaxis()->SetLabelSize(0.09);
    G4_EnergyResponse_ratio->GetYaxis()->SetTitleSize(0.09);G4_EnergyResponse_ratio->GetYaxis()->SetTitleOffset(0.65);
    G4_EnergyResponse_ratio->GetYaxis()->SetRangeUser(0.95,1.05);
    G4_EnergyResponse_ratio->SetName("Pions_ATLASenergyresponseRatio");
    G4_EnergyResponse_ratio->Draw("AP");
    G4_EnergyResponse_ratio -> SetMarkerColor(kRed);
    G4_EnergyResponse_ratio -> SetLineColor(kRed);
    G4_EnergyResponse_ratio->Write();

    outputfile->cd();

    //#######---------Plotting-----------#######

    C1res = new TCanvas("ComparisonEnergyResponse", "", 700, 900);
    p1res = new TPad("p1","p1",0.,0.305,1.,1.);  p1res->Draw();
    p2res = new TPad("p2","p2",0.,0.02,1.,0.32); p2res->Draw();
    
    p1res->cd();
    ATLAS_EnergyResponse->SetTitle("");
    ATLAS_EnergyResponse->Draw("AP");
    G4_EnergyResponse->Draw("same P");
    FLUKA_EnergyResponse->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto* Fresponselegend = new TLegend(0.22,0.6,1.-0.51,0.89);
    Fresponselegend->SetHeader("PIONS");
    Fresponselegend->AddEntry(ATLAS_EnergyResponse,
    "Experimental data", "P");
    Fresponselegend->AddEntry(G4_EnergyResponse,
    "GEANT4", "P");
    Fresponselegend->AddEntry(FLUKA_EnergyResponse,
    "GH", "P");
    Fresponselegend->SetLineWidth(0);
    Fresponselegend->Draw("same");
    p1res->SetTitle("Pions Energy Resolution");
    
    p2res->cd();
    gPad->SetLeftMargin(0.15);
    G4_EnergyResponse_ratio->Draw("AP");
    FLUKA_EnergyResponse_ratio->Draw("same P");

    C1res->Write();
    //C1res->SaveAs("plots/PIONS_EnergyResponse_Comparison.pdf");
    delete p1res, p2res;
    delete G4_EnergyResponse, ATLAS_EnergyResponse, FLUKA_EnergyResponse, G4_EnergyResponse_ratio, FLUKA_EnergyResponse_ratio;
    delete C1res;

    //-----------------KAONS-----------------

    //------ATLAS---------
    ATLAS_EnergyResponse = (TGraphErrors*)G4_file->Get("ATLAS_Energy_Response_Kaons");
    for (unsigned int i=0; i<4; i++){
        ATLAS_EnergyResponse->GetPoint(i, xprime, yprime);
        ATLAS_energyresponse[i] = yprime;
        ATLAS_energyresponse_error[i] = ATLAS_EnergyResponse->GetErrorY(i);
    }
    //ATLAS_EnergyRes->SetMarkerStyle(8);
    ATLAS_EnergyResponse -> SetLineColor(kBlue);
    ATLAS_EnergyResponse -> SetMarkerColor(kBlue);
    ATLAS_EnergyResponse -> SetMarkerStyle(kFullCircle);
    ATLAS_EnergyResponse -> GetYaxis()->SetRangeUser(0.74, 0.85);

    //------FLUKA & RATIO---------
    FLUKA_EnergyResponse = (TGraphErrors*)FLUKA_file->Get("Energy_Response_Kaons");
    FLUKA_EnergyResponse -> SetMarkerStyle(kCircle);
    FLUKA_EnergyResponse -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyResponse -> SetLineColor(kCyan -3);
    FLUKA_EnergyResponse -> GetYaxis()->SetRangeUser(0.74, 0.85);
    
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyResponse->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        FLUKA_energyresponse[i] = yprime;
        FLUKA_energyresponse_error[i] = FLUKA_EnergyResponse->GetErrorY(i);
    }
    
    outputfile->cd();
    FLUKA_EnergyResponse_ratio = new TGraphErrors(4);FLUKA_EnergyResponse_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyResponse_ratio->SetPoint(i, xArray[i], FLUKA_energyresponse[i]/ATLAS_energyresponse[i]);
        FLUKA_EnergyResponse_ratio->SetPointError(i, 0.,
                (FLUKA_energyresponse_error[i]/FLUKA_energyresponse[i]+
                 ATLAS_energyresponse_error[i]/ATLAS_energyresponse[i])*FLUKA_energyresponse[i]/ATLAS_energyresponse[i]);
    }
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitle("MC/Data");
    FLUKA_EnergyResponse_ratio->SetMarkerStyle(kCircle);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetLabelSize(0.09);FLUKA_EnergyResponse_ratio->GetXaxis()->SetLabelSize(0.09);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitleSize(0.09);FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitleOffset(0.65);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetRangeUser(0.95,1.05);
    FLUKA_EnergyResponse_ratio->SetName("Kaons_ATLASenergyresponseRatio");
    FLUKA_EnergyResponse_ratio->Draw("AP");
    FLUKA_EnergyResponse_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyResponse_ratio -> SetLineColor(kCyan -3);
    FLUKA_EnergyResponse_ratio->Write();
    
    //------G4StandAlone & RATIO---------
     G4_EnergyResponse = (TGraphErrors*)G4_file->Get("Energy_Response_Kaons");
    G4_EnergyResponse -> SetMarkerStyle(kCircle);
    G4_EnergyResponse -> SetMarkerColor(kRed);
    G4_EnergyResponse -> SetLineColor(kRed);
    G4_EnergyResponse -> GetYaxis()->SetRangeUser(0.74, 0.85);

    for (unsigned int i=0; i<4; i++){
        G4_EnergyResponse->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        G4_energyresponse[i] = yprime;
        G4_energyresponse_error[i] = G4_EnergyResponse->GetErrorY(i);
    }

    outputfile->cd();

    G4_EnergyResponse_ratio = new TGraphErrors(4);G4_EnergyResponse_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        G4_EnergyResponse_ratio->SetPoint(i, xArray[i], G4_energyresponse[i]/ATLAS_energyresponse[i]);
        G4_EnergyResponse_ratio->SetPointError(i, 0.,
                (G4_energyresponse_error[i]/G4_energyresponse[i]+
                 ATLAS_energyresponse_error[i]/ATLAS_energyresponse[i])*G4_energyresponse[i]/ATLAS_energyresponse[i]);
    }
    G4_EnergyResponse_ratio->GetYaxis()->SetTitle("MC/Data");
    G4_EnergyResponse_ratio->SetMarkerStyle(kCircle);
    G4_EnergyResponse_ratio->GetYaxis()->SetLabelSize(0.09);G4_EnergyResponse_ratio->GetXaxis()->SetLabelSize(0.09);
    G4_EnergyResponse_ratio->GetYaxis()->SetTitleSize(0.09);G4_EnergyResponse_ratio->GetYaxis()->SetTitleOffset(0.65);
    G4_EnergyResponse_ratio->GetYaxis()->SetRangeUser(0.95,1.05);
    G4_EnergyResponse_ratio->SetName("Kaons_ATLASenergyresponseRatio");
    G4_EnergyResponse_ratio->Draw("AP");
    G4_EnergyResponse_ratio -> SetMarkerColor(kRed);
    G4_EnergyResponse_ratio -> SetLineColor(kRed);
    G4_EnergyResponse_ratio->Write();

    outputfile->cd();

    //#######---------Plotting-----------#######

    C1res = new TCanvas("ComparisonEnergyResponse", "", 700, 900);
    p1res = new TPad("p1","p1",0.,0.305,1.,1.);  p1res->Draw();
    p2res = new TPad("p2","p2",0.,0.02,1.,0.32); p2res->Draw();
    
    p1res->cd();
    ATLAS_EnergyResponse->SetTitle("");
    ATLAS_EnergyResponse->Draw("AP");
    G4_EnergyResponse->Draw("same P");
    FLUKA_EnergyResponse->Draw("same P");
    gPad->SetLeftMargin(0.15);
    Fresponselegend = new TLegend(0.22,0.6,1.-0.51,0.89);
    Fresponselegend->SetHeader("KAONS");
    Fresponselegend->AddEntry(ATLAS_EnergyResponse,
    "Experimental data", "P");
    Fresponselegend->AddEntry(G4_EnergyResponse,
    "GEANT4", "P");
    Fresponselegend->AddEntry(FLUKA_EnergyResponse,
    "GH", "P");
    Fresponselegend->SetLineWidth(0);
    Fresponselegend->Draw("same");
    p1res->SetTitle("Kaons Energy Resolution");
    
    p2res->cd();
    gPad->SetLeftMargin(0.15);
    G4_EnergyResponse_ratio->Draw("AP");
    FLUKA_EnergyResponse_ratio->Draw("same P");

    C1res->Write();
    //C1res->SaveAs("plots/KAONS_EnergyResponse_Comparison.pdf");
    delete p1res, p2res;
    delete G4_EnergyResponse, ATLAS_EnergyResponse, FLUKA_EnergyResponse, G4_EnergyResponse_ratio, FLUKA_EnergyResponse_ratio;
    delete C1res;

    //-----------------PROTONS-----------------

    //------ATLAS---------
    ATLAS_EnergyResponse = (TGraphErrors*)G4_file->Get("ATLAS_Energy_Response_Protons");
    for (unsigned int i=0; i<4; i++){
        ATLAS_EnergyResponse->GetPoint(i, xprime, yprime);
        ATLAS_energyresponse[i] = yprime;
        ATLAS_energyresponse_error[i] = ATLAS_EnergyResponse->GetErrorY(i);
    }
    //ATLAS_EnergyRes->SetMarkerStyle(8);
    ATLAS_EnergyResponse -> SetLineColor(kBlue);
    ATLAS_EnergyResponse -> SetMarkerColor(kBlue);
    ATLAS_EnergyResponse -> SetMarkerStyle(kFullCircle);
    ATLAS_EnergyResponse -> GetYaxis()->SetRangeUser(0.7, 0.78);

    //------FLUKA & RATIO---------
    FLUKA_EnergyResponse = (TGraphErrors*)FLUKA_file->Get("Energy_Response_Protons");
    FLUKA_EnergyResponse -> SetMarkerStyle(kCircle);
    FLUKA_EnergyResponse -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyResponse -> SetLineColor(kCyan -3);
    FLUKA_EnergyResponse -> GetYaxis()->SetRangeUser(0.7, 0.78);
    
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyResponse->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        FLUKA_energyresponse[i] = yprime;
        FLUKA_energyresponse_error[i] = FLUKA_EnergyResponse->GetErrorY(i);
    }
    
    outputfile->cd();
    FLUKA_EnergyResponse_ratio = new TGraphErrors(4);FLUKA_EnergyResponse_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        FLUKA_EnergyResponse_ratio->SetPoint(i, xArray[i], FLUKA_energyresponse[i]/ATLAS_energyresponse[i]);
        FLUKA_EnergyResponse_ratio->SetPointError(i, 0.,
                (FLUKA_energyresponse_error[i]/FLUKA_energyresponse[i]+
                 ATLAS_energyresponse_error[i]/ATLAS_energyresponse[i])*FLUKA_energyresponse[i]/ATLAS_energyresponse[i]);
    }
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitle("MC/Data");
    FLUKA_EnergyResponse_ratio->SetMarkerStyle(kCircle);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetLabelSize(0.09);FLUKA_EnergyResponse_ratio->GetXaxis()->SetLabelSize(0.09);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitleSize(0.09);FLUKA_EnergyResponse_ratio->GetYaxis()->SetTitleOffset(0.65);
    FLUKA_EnergyResponse_ratio->GetYaxis()->SetRangeUser(0.95,1.05);
    FLUKA_EnergyResponse_ratio->SetName("Protons_ATLASenergyresponseRatio");
    FLUKA_EnergyResponse_ratio->Draw("AP");
    FLUKA_EnergyResponse_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyResponse_ratio -> SetLineColor(kCyan -3);
    FLUKA_EnergyResponse_ratio->Write();
    
    //------G4StandAlone & RATIO---------
     G4_EnergyResponse = (TGraphErrors*)G4_file->Get("Energy_Response_Protons");
    G4_EnergyResponse -> SetMarkerStyle(kCircle);
    G4_EnergyResponse -> SetMarkerColor(kRed);
    G4_EnergyResponse -> SetLineColor(kRed);
    G4_EnergyResponse -> GetYaxis()->SetRangeUser(0.7, 0.78);

    for (unsigned int i=0; i<4; i++){
        G4_EnergyResponse->GetPoint(i, xprime, yprime);
        xArray[i] = xprime;
        G4_energyresponse[i] = yprime;
        G4_energyresponse_error[i] = G4_EnergyResponse->GetErrorY(i);
    }

    outputfile->cd();

    G4_EnergyResponse_ratio = new TGraphErrors(4);G4_EnergyResponse_ratio->SetTitle("");
    for (unsigned int i=0; i<4; i++){
        G4_EnergyResponse_ratio->SetPoint(i, xArray[i], G4_energyresponse[i]/ATLAS_energyresponse[i]);
        G4_EnergyResponse_ratio->SetPointError(i, 0.,
                (G4_energyresponse_error[i]/G4_energyresponse[i]+
                 ATLAS_energyresponse_error[i]/ATLAS_energyresponse[i])*G4_energyresponse[i]/ATLAS_energyresponse[i]);
    }
    G4_EnergyResponse_ratio->GetYaxis()->SetTitle("MC/Data");
    G4_EnergyResponse_ratio->SetMarkerStyle(kCircle);
    G4_EnergyResponse_ratio->GetYaxis()->SetLabelSize(0.09);G4_EnergyResponse_ratio->GetXaxis()->SetLabelSize(0.09);
    G4_EnergyResponse_ratio->GetYaxis()->SetTitleSize(0.09);G4_EnergyResponse_ratio->GetYaxis()->SetTitleOffset(0.65);
    G4_EnergyResponse_ratio->GetYaxis()->SetRangeUser(0.95,1.05);
    G4_EnergyResponse_ratio->SetName("Protons_ATLASenergyresponseRatio");
    G4_EnergyResponse_ratio->Draw("AP");
    G4_EnergyResponse_ratio -> SetMarkerColor(kRed);
    G4_EnergyResponse_ratio -> SetLineColor(kRed);
    G4_EnergyResponse_ratio->Write();

    outputfile->cd();

    //#######---------Plotting-----------#######

    C1res = new TCanvas("ComparisonEnergyResponse", "", 700, 900);
    p1res = new TPad("p1","p1",0.,0.305,1.,1.);  p1res->Draw();
    p2res = new TPad("p2","p2",0.,0.02,1.,0.32); p2res->Draw();
    
    p1res->cd();
    ATLAS_EnergyResponse->SetTitle("");
    ATLAS_EnergyResponse->Draw("AP");
    G4_EnergyResponse->Draw("same P");
    FLUKA_EnergyResponse->Draw("same P");
    gPad->SetLeftMargin(0.15);
    Fresponselegend = new TLegend(0.22,0.6,1.-0.51,0.89);
    Fresponselegend->SetHeader("PROTONS");
    Fresponselegend->AddEntry(ATLAS_EnergyResponse,
    "Experimental data", "P");
    Fresponselegend->AddEntry(G4_EnergyResponse,
    "GEANT4", "P");
    Fresponselegend->AddEntry(FLUKA_EnergyResponse,
    "GH", "P");
    Fresponselegend->SetLineWidth(0);
    Fresponselegend->Draw("same");
    p1res->SetTitle("Protons Energy Resolution");
    
    p2res->cd();
    gPad->SetLeftMargin(0.15);
    G4_EnergyResponse_ratio->Draw("AP");
    FLUKA_EnergyResponse_ratio->Draw("same P");

    C1res->Write();
    //C1res->SaveAs("plots/PROTONS_EnergyResponse_Comparison.pdf");
    delete p1res, p2res;
    delete G4_EnergyResponse, ATLAS_EnergyResponse, FLUKA_EnergyResponse, G4_EnergyResponse_ratio, FLUKA_EnergyResponse_ratio;
    delete C1res;

}
#endif