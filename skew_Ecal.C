double skewedgauss(double *x, double *p) {
    double xi = p[0];     // location (mean)
    double omega = p[1];  // scale (sigma-like)
    double alpha = p[2];  // skew
    double arg = (x[0] - xi) / omega;
    double smallphi = TMath::Gaus(arg, 0.0, 1.0, true); // true = normalized
    double bigphi = 0.5 * (1 + std::erf(alpha * arg / std::sqrt(2)));
    return 2.0 / omega * smallphi * bigphi;
}

void skew_Ecal() {
    gStyle->SetOptStat(0);

    TFile *file = TFile::Open("e_0.5GeV_Ecal_1e4.edm4hep.root");
    TTree *events = (TTree*)file->Get("events");

    TCanvas *c1 = new TCanvas("c1", "Energy Histogram", 1000, 800);

    // Updated histogram range for peak region
    events->Draw("Sum$(EcalEndcapNHits.energy) >> htemp(22, 0.4, 0.5)");

    TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    htemp->SetTitle("Energy deposited in the Ecal by 0.5 GeV e-;Energy (GeV);Counts");
    htemp->SetLineColor(kBlue + 1);
    htemp->SetFillColor(kAzure - 9);
    htemp->SetFillStyle(3001);

    // Define skewed Gaussian TF1 with correct parameter count (3)
    TF1 *skewFit = new TF1("skewFit", skewedgauss, 0.4, 0.495, 3);
    skewFit->SetParNames("location", "scale", "shape");

    // Improved initial parameters
    skewFit->SetParameters(0.485, 0.015, -5.0);
    skewFit->SetLineColor(kRed);

    // Set parameter limits to guide the fit
    skewFit->SetParLimits(0, 0.47, 0.495);  // location
    skewFit->SetParLimits(1, 0.01, 0.03);  // scale (sigma)
    skewFit->SetParLimits(2, -10.0, 10.0);  // skew

    // Perform the fit
    htemp->Fit(skewFit, "R");

    // Draw histogram and fit
    htemp->Draw("HIST");
    skewFit->Draw("same");

    // Get fitted parameters
    double location = skewFit->GetParameter(0);
    double scale = skewFit->GetParameter(1);
    double shape = skewFit->GetParameter(2);

    // Add legend
    TLegend *legend = new TLegend(0.55, 0.78, 0.83, 0.91);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.035);
    legend->AddEntry(skewFit, Form("#mu=%.3f, #sigma=%.3f, #alpha=%.2f", location, scale, shape), "l");
    legend->Draw();

    c1->Update();
    c1->SaveAs("ecal_0.5GeV_skewfit.png");
}