#include "global.h"

#include "windows.h"
#include "WinBase.h"

#include <boost/filesystem.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

using namespace std;

Mode mode = Sequence;

bool isValidPlugin(const char *path)
{
    boost::filesystem::path shared_library_path = path;
    boost::function<pluginapi_create_t> plugin_creator;

    try {
        plugin_creator = boost::dll::import_alias<pluginapi_create_t>(
            shared_library_path,
            "create_plugin",
            boost::dll::load_mode::load_with_altered_search_path
        );
    } catch (.../*boost::system::system_error &e*/) {
        return false;
    }

    return true;
}

void searchPlugin(const char *path,list<string> &plugin)
{
    typedef boost::filesystem::recursive_directory_iterator dir_iter;
    dir_iter end;

    boost::filesystem::path shared_library_path;

    boost::winapi::UINT_ errorMode = GetErrorMode();
    SetErrorMode(boost::winapi::SEM_FAILCRITICALERRORS_);

    for (dir_iter pos(path);pos != end;++pos) {
        if (boost::filesystem::is_directory(*pos)) {
            continue;
        }

        DLL_DIRECTORY_COOKIE cookie = AddDllDirectory(pos->path().parent_path().c_str());

        if (isValidPlugin(pos->path().string().c_str())) {
            plugin.push_back(pos->path().string());
        } else {
            RemoveDllDirectory(cookie);
        }
    }

    SetErrorMode(errorMode);
}
