#include "app/config.h"

#include <filesystem>
#include <fstream>
#include <iostream>

int main()
{
    const auto path = std::filesystem::temp_directory_path() /
        "nitlink-custom-no-signal-config-test.txt";
    std::error_code ec;
    std::filesystem::remove(path, ec);

    // A pre-E1 config with none of the new keys keeps compiled defaults.
    {
        std::ofstream legacy(path, std::ios::trunc);
        legacy << "window_width = 1280\n";
    }
    NitLink::Config legacyLoaded;
    if (!legacyLoaded.Load(path.string())) return 1;
    if (legacyLoaded.noSignalMode != "default" ||
        !legacyLoaded.noSignalImage.empty() ||
        legacyLoaded.noSignalFit != "contain" ||
        !legacyLoaded.noSignalDimImage ||
        !legacyLoaded.discordRpcEnabled) return 2;

    NitLink::Config defaults;
    if (!defaults.Save(path.string())) return 3;
    NitLink::Config defaultLoaded;
    if (!defaultLoaded.Load(path.string())) return 4;
    if (defaultLoaded.noSignalMode != "default" ||
        !defaultLoaded.noSignalImage.empty() ||
        defaultLoaded.noSignalFit != "contain" ||
        !defaultLoaded.noSignalDimImage ||
        !defaultLoaded.discordRpcEnabled) return 5;

    NitLink::Config saved;
    saved.noSignalMode = "image";
    saved.noSignalImage =
        "C:\\Users\\測試者\\Pictures\\無訊號圖片.png";
    saved.noSignalFit = "cover";
    saved.noSignalDimImage = false;
    saved.discordRpcEnabled = false;

    NitLink::CaptureFormatOverride switchFormat;
    switchFormat.width = 1920;
    switchFormat.height = 1080;
    switchFormat.fps = 60;
    switchFormat.fpsNumerator = 60000;
    switchFormat.fpsDenominator = 1001;
    switchFormat.format = L"NV12";
    saved.captureFormatOverrides[L"USB Video (Nintendo Switch)"] = switchFormat;

    NitLink::CaptureFormatOverride genericFormat;
    genericFormat.width = 3840;
    genericFormat.height = 2160;
    genericFormat.fps = 30;
    genericFormat.fpsNumerator = 30;
    genericFormat.fpsDenominator = 1;
    genericFormat.format = L"BGRA";
    saved.captureFormatOverrides[L"Generic HDMI Capture"] = genericFormat;
    if (!saved.Save(path.string())) return 6;

    NitLink::Config loaded;
    if (!loaded.Load(path.string())) return 7;
    if (loaded.noSignalMode != saved.noSignalMode ||
        loaded.noSignalImage != saved.noSignalImage ||
        loaded.noSignalFit != saved.noSignalFit ||
        loaded.noSignalDimImage != saved.noSignalDimImage ||
        loaded.discordRpcEnabled ||
        loaded.captureFormatOverrides.size() != 2) return 8;
    const auto loadedSwitch = loaded.GetOverride(L"USB Video (Nintendo Switch)");
    const auto loadedGeneric = loaded.GetOverride(L"Generic HDMI Capture");
    if (loadedSwitch.width != 1920 || loadedSwitch.height != 1080 ||
        loadedSwitch.fpsNumerator != 60000 || loadedSwitch.fpsDenominator != 1001 ||
        loadedSwitch.format != L"NV12" || loadedGeneric.width != 3840 ||
        loadedGeneric.height != 2160 || loadedGeneric.fps != 30 ||
        loadedGeneric.format != L"BGRA") return 18;

    saved.noSignalFit = "stretch";
    saved.noSignalDimImage = true;
    if (!saved.Save(path.string())) return 9;
    NitLink::Config stretchLoaded;
    if (!stretchLoaded.Load(path.string()) ||
        stretchLoaded.noSignalFit != "stretch" ||
        !stretchLoaded.noSignalDimImage) return 10;

    // Switching back to the branded page preserves the selected image path.
    // Only the explicit remove-image action clears it.
    saved.noSignalMode = "default";
    if (!saved.Save(path.string())) return 11;
    NitLink::Config defaultWithImageLoaded;
    if (!defaultWithImageLoaded.Load(path.string()) ||
        defaultWithImageLoaded.noSignalMode != "default" ||
        defaultWithImageLoaded.noSignalImage != saved.noSignalImage) return 12;

    saved.noSignalImage.clear();
    if (!saved.Save(path.string())) return 13;
    NitLink::Config clearedLoaded;
    if (!clearedLoaded.Load(path.string()) ||
        clearedLoaded.noSignalMode != "default" ||
        !clearedLoaded.noSignalImage.empty()) return 14;

    {
        std::ofstream invalid(path, std::ios::trunc);
        invalid << "no_signal_mode = unsupported\n"
                << "no_signal_fit = unsupported\n"
                << "no_signal_image = C:\\Users\\測試者\\無訊號.bmp\n";
    }
    NitLink::Config invalidLoaded;
    if (!invalidLoaded.Load(path.string())) return 15;
    if (invalidLoaded.noSignalMode != "default" ||
        invalidLoaded.noSignalFit != "contain") return 16;

    if (!std::filesystem::remove(path, ec) && std::filesystem::exists(path)) {
        return 17;
    }

    std::cout << "custom No Signal config tests passed\n";
    return 0;
}
