#pragma once

#include <memory>
#include <vector>
#include "TFile.h"
#include "ParserFactory.h"
#include <algorithm>
#include "TSpectrumTransform.h"

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

        // TODO
        template<typename T> double Integrate(std::vector<T> waveform) {
            double s = 0;
            for (int i = 0; i < waveform.size(); i++) {
                s += std::abs(waveform[i]/2.);
            }
            return std::abs(s);
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
    };
}