#include <consts.hpp>
#include <base.hpp>

#include "policy.hpp"

using namespace std;

void sepolicy::magisk_rules() {
    // Temp suppress warnings
    set_log_level_state(LogLevel::Warn, false);

    type(SEPOL_PROC_DOMAIN, "domain");
    permissive(SEPOL_PROC_DOMAIN);  /* Just in case something is missing */

    typeattribute(SEPOL_PROC_DOMAIN, "mlstrustedsubject");
    typeattribute(SEPOL_PROC_DOMAIN, "netdomain");
    typeattribute(SEPOL_PROC_DOMAIN, "bluetoothdomain");

    type(SEPOL_FILE_TYPE, "file_type");
    typeattribute(SEPOL_FILE_TYPE, "mlstrustedobject");

    allow(SEPOL_PROC_DOMAIN, ALL, ALL, ALL);

    // Allow us to do any ioctl
    if (impl->db->policyvers >= POLICYDB_VERSION_XPERMS_IOCTL) {
        argument all;
        all.first.push_back(nullptr);
        allowxperm(SEPOL_PROC_DOMAIN, ALL, "blk_file", all);
        allowxperm(SEPOL_PROC_DOMAIN, ALL, "fifo_file", all);
        allowxperm(SEPOL_PROC_DOMAIN, ALL, "chr_file", all);
        allowxperm(SEPOL_PROC_DOMAIN, ALL, "file", all);
    }

    // suRights
    allow("servicemanager", SEPOL_PROC_DOMAIN, "dir", "search");
    allow("servicemanager", SEPOL_PROC_DOMAIN, "dir", "read");
    allow("servicemanager", SEPOL_PROC_DOMAIN, "file", "open");
    allow("servicemanager", SEPOL_PROC_DOMAIN, "file", "read");
    allow("servicemanager", SEPOL_PROC_DOMAIN, "process", "getattr");
    allow(ALL, SEPOL_PROC_DOMAIN, "process", "sigchld");

    // allowLog
    allow("logd", SEPOL_PROC_DOMAIN, "dir", "search");
    allow("logd", SEPOL_PROC_DOMAIN, "file", "read");
    allow("logd", SEPOL_PROC_DOMAIN, "file", "open");
    allow("logd", SEPOL_PROC_DOMAIN, "file", "getattr");

    // dumpsys
    allow(ALL, SEPOL_PROC_DOMAIN, "fd", "use");
    allow(ALL, SEPOL_PROC_DOMAIN, "fifo_file", "write");
    allow(ALL, SEPOL_PROC_DOMAIN, "fifo_file", "read");
    allow(ALL, SEPOL_PROC_DOMAIN, "fifo_file", "open");
    allow(ALL, SEPOL_PROC_DOMAIN, "fifo_file", "getattr");

    // bootctl
    allow("hwservicemanager", SEPOL_PROC_DOMAIN, "dir", "search");
    allow("hwservicemanager", SEPOL_PROC_DOMAIN, "file", "read");
    allow("hwservicemanager", SEPOL_PROC_DOMAIN, "file", "open");
    allow("hwservicemanager", SEPOL_PROC_DOMAIN, "process", "getattr");

    // For mounting loop devices, mirrors, tmpfs
    allow("kernel", ALL, "file", "read");
    allow("kernel", ALL, "file", "write");

    // Allow all binder transactions
    allow(ALL, SEPOL_PROC_DOMAIN, "binder", ALL);

    // Allow system server devpts
    allow("system_server", "untrusted_app_all_devpts", "chr_file","read");
    allow("system_server", "untrusted_app_all_devpts", "chr_file","write");

#if 0
    // Remove all dontaudit in debug mode
    impl->strip_dontaudit();
#endif

    set_log_level_state(LogLevel::Warn, true);
}
