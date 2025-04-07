void plot_Ecal() {
    gStyle->SetOptStat(0);  // No stat box

    TFile *file = TFile::Open("e_1GeV_Ecal_1e4.edm4hep.root"); // Open the ROOT file
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

    double lowLim = 0.93;
    double upLim = 0.99;

    events->Draw("Sum$(EcalEndcapNHits.energy) >> htemp(25, 0.93, 0.99");

    TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    htemp->SetTitle("Energy deposited in the Ecal by 1 GeV e-;Energy (GeV);Counts");
    htemp->SetLineColor(kBlue + 1);
    htemp->SetFillColor(kAzure - 9);
    htemp->SetFillStyle(3001);

    
    //TF1 *gausFit = new TF1("gausFit", "gaus", lowLim, upLim);
    
    // Set initial parameters: [0]=amplitude, [1]=mean, [2]=sigma
    //gausFit->SetParameters(htemp->GetMaximum(), 0.085, 0.005); 
    //gausFit->SetLineColor(kRed);
    //htemp->Fit(gausFit, "R");

    int peakBin = htemp->GetMaximumBin();
    double center = htemp->GetBinCenter(peakBin);
    int peakAmplitude = htemp->GetMaximum();

    std::cout << "Curve Peak: " << peakAmplitude << " At: "<< center << std::endl;

    int bin1 = htemp->FindFirstBinAbove(htemp->GetMaximum()/2);
    int bin2 = htemp->FindLastBinAbove(htemp->GetMaximum()/2);
    double fwhm = htemp->GetBinCenter(bin2) - htemp->GetBinCenter(bin1);

    std::cout << "Bin 1 center: " << htemp->GetBinCenter(bin1) << std::endl;
    std::cout << "Bin 2 center: " << htemp->GetBinCenter(bin2) << std::endl;

    double stdDev = fwhm/2.35482;

    std::cout << "FWHM: " << fwhm << std::endl;

    // Draw histogram and fit
    htemp->Draw("HIST");

    // Add a manually-defined Gaussian curve for comparison
    double customAmplitude = peakAmplitude;   // You can adjust this
    double customMean = center;
    //double customMean = 0.5 * (htemp->GetBinCenter(bin1) + htemp->GetBinCenter(bin2));
    double customSigma = stdDev;

    // Define the Gaussian shape
    TF1 *customGauss = new TF1("customGauss", "[0]*exp(-0.5*((x-[1])/[2])**2)", lowLim, upLim);
    customGauss->SetParameters(customAmplitude, customMean, customSigma);
    customGauss->SetLineColor(kGreen+2);
    //customGauss->SetLineStyle(2);  // Dashed line for visual distinction
    customGauss->SetLineWidth(2);
    customGauss->Draw("same");

    //gausFit->Draw("same");

    // Count entries between the ploting limits
    int binLow = htemp->FindBin(lowLim);
    int binHigh = htemp->FindBin(upLim);
    int entriesInRange = htemp->Integral(binLow, binHigh);

    std::cout << "Number of entries: " << entriesInRange << std::endl;

    // Display entry count on canvas
    // TLatex latex;
    // latex.SetNDC();
    // latex.SetTextSize(0.04);
    // latex.DrawLatex(0.15, 0.85, Form("Entries [4.84,4.95] GeV: %d", entriesInRange));

    // // Add legend with fit parameters
    //TLegend *legend = new TLegend(0.57, 0.80, 0.83, 0.91); // x1,y1,x2,y2
    //TLegend *legend = new TLegend(0.15, 0.78, 0.43, 0.91);  // x1, y1, x2, y2
    TLegend *legend = new TLegend(0.12, 0.15, 0.42, 0.28);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0); // transparent
    legend->SetTextSize(0.035);

    //double mean = gausFit->GetParameter(1);
    //double sigma = gausFit->GetParameter(2);
    //legend->AddEntry(htemp, "Ecal Endcap Energy", "f");
    //legend->AddEntry(gausFit, Form("#mu=%.3f, #sigma=%.3f", mean, sigma), "l");
    //legend->AddEntry(customGauss, Form("Peak=%.3f GeV, FWHM=%.3f GeV", center, fwhm), "l");
    legend->AddEntry(customGauss, Form("Center=%.4f GeV, FWHM=%.4f GeV", center, fwhm), "l");

    legend->Draw();

    c1->Update();
    c1->SaveAs("ecal_1GeV_FWHM_wGauss_peak.png");
}