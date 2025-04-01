void plot_Ecal() {
    gStyle->SetOptStat(0);  // No stat box

    TFile *file = TFile::Open("e_0.1GeV_Ecal_1e4.edm4hep.root"); // Open the ROOT file
    // if (!file || file->IsZombie()) {
    //    std::cerr << "Error: Could not open the file!" << std::endl;
    //    return;
    // }

    TTree *events = (TTree*)file->Get("events");
    // if (!events) {
    //    std::cerr << "Error: 'events' tree not found!" << std::endl;
    //    return;
    // }

    TCanvas *c1 = new TCanvas("c1", "Energy Histogram", 1000, 800);

    events->Draw("Sum$(EcalEndcapNHits.energy) >> htemp(30, 0.07, 0.095)");

    TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    htemp->SetTitle("Energy deposited in the Ecal by 0.1 GeV e-;Energy (GeV);Counts");
    htemp->SetLineColor(kBlue + 1);
    htemp->SetFillColor(kAzure - 9);
    htemp->SetFillStyle(3001);

    TF1 *gausFit = new TF1("gausFit", "gaus", 0.0825, 0.09);
    
    // Set initial parameters: [0]=amplitude, [1]=mean, [2]=sigma
    gausFit->SetParameters(htemp->GetMaximum(), 0.09, 0.2); 
    gausFit->SetLineColor(kRed);
    htemp->Fit(gausFit, "R");

    std::cout << htemp->GetMaximum() << std::endl;

    int bin1 = htemp->FindFirstBinAbove(htemp->GetMaximum()/2);
    int bin2 = htemp->FindLastBinAbove(htemp->GetMaximum()/2);
    double fwhm = htemp->GetBinCenter(bin2) - htemp->GetBinCenter(bin1);
    double stdDev = fwhm/2.35482;

    std::cout << "Sigma by FWHM: "<< stdDev << std::endl;

    // Draw histogram and fit
    htemp->Draw("HIST");
    gausFit->Draw("same");

    // Count entries between 9.6 and 9.9 GeV
    // int binLow = htemp->FindBin(9.6);
    // int binHigh = htemp->FindBin(9.9);
    // int entriesInRange = htemp->Integral(binLow, binHigh);

    // Display entry count on canvas
    // TLatex latex;
    // latex.SetNDC();
    // latex.SetTextSize(0.04);
    // latex.DrawLatex(0.15, 0.85, Form("Entries [4.84,4.95] GeV: %d", entriesInRange));

    // // Add legend with fit parameters
    TLegend *legend = new TLegend(0.57, 0.80, 0.83, 0.91); // x1,y1,x2,y2
    legend->SetBorderSize(0);
    legend->SetFillStyle(0); // transparent
    legend->SetTextSize(0.035);

    double mean = gausFit->GetParameter(1);
    double sigma = gausFit->GetParameter(2);
    //legend->AddEntry(htemp, "Ecal Endcap Energy", "f");
    legend->AddEntry(gausFit, Form("#mu=%.3f, #sigma=%.3f", mean, sigma), "l");

    legend->Draw();

    c1->Update();
    c1->SaveAs("ecal_0.1GeV.png");
}