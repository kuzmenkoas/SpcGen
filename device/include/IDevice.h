#pragma once

#include <memory>
#include <vector>
#include "TFile.h"
#include "ParserFactory.h"
#include <algorithm>
#include "TSpectrumTransform.h"
#include "TGraph.h"
#include <TF1.h>
#include <mutex>

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

        template<typename T> std::vector<double> NormalizeWaveform(std::vector<T> waveform);
        template<typename T> std::vector<double> CCF(std::vector<T> w1, std::vector<T> w2);
        template<typename T> double TemplateCalculateAmplitude(std::vector<T> eventWaveform, double baseline);
        template<typename T> double TemplateCalculateBaseline(std::vector<T> eventWaveform);
        template<typename T> double TemplateCalculateCharge(std::vector<T> eventWaveform, double baseline);
        template<typename T> bool ThresholdSignalFilter(std::vector<T> eventWaveform, double baseline);
        template<typename T1, typename T2> bool CCFSignalFilter(std::vector<T1> eventWaveform, std::vector<T2> averageWaveform);
        template<typename T1, typename T2> bool SignalFilter(std::vector<T1> eventWaveform, std::vector<T2> averageWaveform, double baseline);
        template<typename T> void TemplateCalculateWaveform(std::vector<T> eventWaveform, std::vector<double>* averageWaveform);
        void DefineSignalDirection(std::vector<double> averageWaveform);
        double MaxCCF(std::vector<double> ccfvector);
    private:
        template<typename T> void VectorLengthCorrect(std::vector<T>& eventWaveform);
        template<typename T> void TemplateInitializeSumWaveform(std::vector<T> eventWaveform, std::vector<double>* averageWaveform);
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

        mutable std::once_flag initWaveFlag;
        bool isInitialized = false;
    };
}

#include "IDevice.icpp"