#pragma once

#include <memory>
#include <vector>
#include "TFile.h"
#include "ParserFactory.h"
#include <algorithm>
#include "TSpectrumTransform.h"
#include "TGraph.h"
#include <TF1.h>

namespace Device {
    class IDevice {
    public:
        IDevice();
        ~IDevice();

        void SetConfigPath(std::filesystem::path path) {fConfigPath = path;};
        std::filesystem::path GetConfigPath() {return fConfigPath;};

        void SetBinaryPathVector(std::vector<std::filesystem::path> pathVector) {fBinaryPathVector = pathVector;};
        std::vector<std::filesystem::path> GetBinaryPathVector() {return fBinaryPathVector;};
        
        void SetFileName(std::string name) {fFileName = name;};
        std::string GetFileName() {return fFileName;};

        void SetDeviceType(Global::DeviceType aType) {fDeviceType = aType;};

        std::shared_ptr<Parser::IParser> GetParser() {return fParser;};
        void Prepare();
        virtual void PrepareDevice() {};
        virtual void Start() {};

        void SetDigitizerTypes(std::vector<std::string> aDigitizerTypes) {fDigitizerTypes = aDigitizerTypes;};
        std::vector<std::string> GetDigitizerTypes() {return fDigitizerTypes;};

        TFile* fRootFile = nullptr;

        void SetIsCut(bool val) {bCut = val;};
        bool GetIsCut() {return bCut;};
        void SetIsDebug(bool val) {bDebug = val;};
        bool GetIsDebug() {return bDebug;};
        void SetIsThreshold(bool val) {bThreshold = val;};
        bool GetIsThreshold() {return bThreshold;};

        template<typename T> std::vector<double> NormalizeWaveform(std::vector<T> waveform) {
            auto valMax = std::max_element(waveform.begin(), waveform.end());
            auto valMin = std::min_element(waveform.begin(), waveform.end());

            if (std::abs(*valMax) < std::abs(*valMin)) valMax = valMin;
            std::vector<double> nWaveform = {};
            for (auto &val : waveform) nWaveform.push_back(val/(*valMax));

            return nWaveform;
        };

        template<typename T> std::vector<double> CCF(std::vector<T> w1, std::vector<T> w2) {
            TSpectrumTransform* tspct = new TSpectrumTransform(w1.size());
            tspct->SetTransformType(TSpectrumTransform::kTransformFourier, 0);
            tspct->SetDirection(TSpectrumTransform::kTransformForward);
            std::vector<T> lvalue;
            std::vector<T> rvalue;
            lvalue.resize(2*w1.size());
            rvalue.resize(2*w1.size());
            tspct->Transform(w1.data(), lvalue.data());
            tspct->Transform(w2.data(), rvalue.data());
            for (int i = 0; i < lvalue.size(); i++) lvalue[i] *= rvalue[i];
            rvalue.clear();
            rvalue.resize(w1.size());
            tspct->SetDirection(TSpectrumTransform::kTransformInverse);
            tspct->Transform(lvalue.data(), rvalue.data());
            for (int i = 0; i < rvalue.size(); i++) rvalue[i] /= lvalue[i];
            return rvalue;
        };

        template<typename T> double TemplateCalculateAmplitude(std::vector<T> eventWaveform, double baseline) {
            Global::Parameters usedParameters = GetParser()->GetUsedParameters();

            double amplitude = 0;
            TF1* parabola = new TF1("parabola", "pol2", usedParameters.signalRange.value().first, usedParameters.signalRange.value().second);
            TGraph* gr = new TGraph();
            for (size_t i = 0; i < eventWaveform.size(); i++) {
                gr->SetPoint(i, i, eventWaveform[i]);
            }
            gr->Fit("parabola", "QR");
            double x = -parabola->GetParameter(1)/(2*parabola->GetParameter(2));
            amplitude = parabola->GetParameter(2)*x*x+parabola->GetParameter(1)*x+parabola->GetParameter(0)-baseline;
            double factor = 1;
            double shift = 0;
            if (usedParameters.factorAmplitude.has_value()) factor = usedParameters.factorAmplitude.value();
            if (usedParameters.shiftAmplitude.has_value()) shift = usedParameters.shiftAmplitude.value();
            amplitude = amplitude * factor + shift;
            return amplitude;
        };

        template<typename T> double TemplateCalculateBaseline(std::vector<T> eventWaveform) {
            Global::Parameters usedParameters = GetParser()->GetUsedParameters();
            int min = usedParameters.baselineLimits.value().first;
            int max = usedParameters.baselineLimits.value().second;

            double ss = 0;
            for (size_t i = 0; i < size(eventWaveform); i++) {
                if ((i >= min) && (i <= max)) {
                    ss += eventWaveform[i];
                }
            }
            return ss/(max-min+1.);
        };

        template<typename T> double TemplateCalculateCharge(std::vector<T> eventWaveform, double baseline) {
            Global::Parameters usedParameters = GetParser()->GetUsedParameters();
            int min = usedParameters.signalRange.value().first;
            int max = usedParameters.signalRange.value().second;
            double charge = 0;
            int counter = 0;
            for (double waveform : eventWaveform) {
                if ((counter >= min) && (counter <= max)) charge += waveform - baseline;
                counter++;
            }
            double factor = 1;
            double shift = 0;
            if (usedParameters.factorCharge.has_value()) factor = usedParameters.factorCharge.value();
            if (usedParameters.shiftCharge.has_value()) shift = usedParameters.shiftCharge.value();
            return charge * factor + shift;
        };

        double MaxCCF(std::vector<double> ccfvector);
    private:
        void ConfigureRoot();
        void ConfigureTxt();
        std::filesystem::path fConfigPath;
        std::vector<std::filesystem::path> fBinaryPathVector{};
        std::shared_ptr<Parser::IParser> fParser = nullptr;
        std::string fFileName;
        Global::DeviceType fDeviceType;
        std::vector<std::string> fDigitizerTypes = {};

        bool bCut = false;
        bool bDebug = false;
        bool bThreshold = false;
    };
}