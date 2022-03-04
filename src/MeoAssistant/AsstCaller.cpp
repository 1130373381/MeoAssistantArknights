#include "AsstCaller.h"

#include <string.h>
#include <iostream>

#include <meojson/json.hpp>

#include "Assistant.h"
#include "AsstDef.h"
#include "Version.h"
#include "Logger.hpp"
#include "Resource.h"

#if 0
#if _MSC_VER
// Win32平台下Dll的入口
BOOL APIENTRY DllMain(HINSTANCE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#elif VA_GNUC

#endif
#endif

static bool inited = false;

bool AsstLoadResource(const char* path)
{
    inited = asst::Logger::set_dirname(std::string(path) + "/")
        && asst::Resrc.load(std::string(path) + "/resource/");
    return inited;
}

AsstHandle AsstCreate()
{
    if (!inited) {
        return nullptr;
    }
    return new asst::Assistant();
}

AsstHandle AsstCreateEx(AsstApiCallback callback, void* custom_arg)
{
    if (!inited) {
        return nullptr;
    }
    return new asst::Assistant(callback, custom_arg);
}

void AsstDestroy(AsstHandle handle)
{
    if (handle == nullptr) {
        return;
    }

    delete handle;
    handle = nullptr;
}

bool AsstConnect(AsstHandle handle, const char* adb_path, const char* address, const char* config)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    return handle->connect(adb_path, address, config ? config : std::string());
}

bool ASSTAPI AsstAppendStartUp(AsstHandle handle)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    return handle->append_start_up();
}

bool AsstAppendFight(AsstHandle handle, const char* stage, int max_mecidine, int max_stone, int max_times)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    return handle->append_fight(stage, max_mecidine, max_stone, max_times);
}

bool AsstAppendAward(AsstHandle handle)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    return handle->append_award();
}

bool AsstAppendVisit(AsstHandle handle)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    return handle->append_visit();
}

bool AsstAppendMall(AsstHandle handle, bool with_shopping)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    return handle->append_mall(with_shopping);
}

//bool AsstAppendProcessTask(AsstHandle handle, const char* task_name)
//{
//    if (!inited || handle == nullptr) {
//        return false;
//    }
//
//    return handle->append_process_task(task_name);
//}

bool AsstStartRecruitCalc(AsstHandle handle, const int select_level[], int required_len, bool set_time)
{
    if (!inited || handle == nullptr) {
        return false;
    }
    std::vector<int> level_vector;
    level_vector.assign(select_level, select_level + required_len);
    return handle->start_recruit_calc(level_vector, set_time);
}

bool AsstAppendInfrast(AsstHandle handle, int work_mode, const char** order, int order_size, const char* uses_of_drones, double dorm_threshold)
{
    if (!inited || handle == nullptr) {
        return false;
    }
    std::vector<std::string> order_vector;
    order_vector.assign(order, order + order_size);

    return handle->append_infrast(
        static_cast<asst::infrast::WorkMode>(work_mode),
        order_vector,
        uses_of_drones,
        dorm_threshold);
}

bool AsstAppendRecruit(AsstHandle handle, int max_times, const int select_level[], int select_len, const int confirm_level[], int confirm_len, bool need_refresh, bool use_expedited)
{
    if (!inited || handle == nullptr) {
        return false;
    }
    std::vector<int> required_vector;
    required_vector.assign(select_level, select_level + select_len);
    std::vector<int> confirm_vector;
    confirm_vector.assign(confirm_level, confirm_level + confirm_len);

    return handle->append_recruit(max_times, required_vector, confirm_vector, need_refresh, use_expedited);
}

bool AsstAppendRoguelike(AsstHandle handle, int mode)
{
    if (!inited || handle == nullptr) {
        return false;
    }
    return handle->append_roguelike(mode);
}

bool AsstStart(AsstHandle handle)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    return handle->start();
}

bool AsstStop(AsstHandle handle)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    return handle->stop();
}

bool AsstSetPenguinId(AsstHandle handle, const char* id)
{
    if (!inited || handle == nullptr) {
        return false;
    }

    handle->set_penguin_id(id);
    return true;
}

unsigned long long AsstGetImage(AsstHandle handle, void* buff, unsigned long long buff_size)
{
    if (!inited || handle == nullptr || buff == nullptr) {
        return 0;
    }
    auto img_data = handle->get_image();
    size_t data_size = img_data.size();
    if (buff_size < data_size) {
        return 0;
    }
    memcpy(buff, img_data.data(), data_size);
    return data_size;
}

bool AsstCtrlerClick(AsstHandle handle, int x, int y, bool block)
{
    if (!inited || handle == nullptr) {
        return false;
    }
    return handle->ctrler_click(x, y, block);
}

//bool AsstSetParam(AsstHandle handle, const char* type, const char* param, const char* value)
//{
//    if (handle == nullptr) {
//        return false;
//    }
//
//    return handle->set_param(type, param, value);
//}

const char* AsstGetVersion()
{
    return asst::Version;
}

void AsstLog(const char* level, const char* message)
{
    if (!inited) {
        std::cerr << "Not inited" << std::endl;
        return;
    }
    asst::Log.log_with_custom_level(level, message);
}

bool AsstAppendDebug(AsstHandle handle)
{
    if (!inited || handle == nullptr) {
        return false;
    }
#ifdef ASST_DEBUG
    return handle->append_debug();
#else
    return false;
#endif // ASST_DEBUG
}
