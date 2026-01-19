#pragma once

#include <memory>
#include <vector>
#include "TFile.h"
#include "ParserFactory.h"
#include <algorithm>

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
            std::vector<double> res = {};
            // for (int i = 0; i < w1.size(); i++) {
                // double s = 0;
                // for (int k = 0; k < w1.size(); k++) {
                //     int idx = i+k;
                //     if (idx >= w1.size()) idx -= w1.size();
                //     s += w1[k]*w2[idx];
                // }
                int n = w1.size();
                int m = w2.size();
                for (int shift = -m+1; shift < n; shift++) {
                    double s = 0.0;
                    int result_idx = shift+m-1;
                    for (int i = 0; i < n; i++) {
                        int j = i-shift;
                        if (j >= 0 && j < m) s += w1[i]*w2[j];
                    }
                    // std::cout << s << std::endl;
                    res.push_back(s);
                // }
            }
            return res;
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