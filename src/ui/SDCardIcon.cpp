#include <core/Logger.h>
#include "SDCardIcon.h"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_SDCARD, "Mgui/Sdcard", "Sdcard");

#ifdef PXA1826
static struct uci_package *config_try_load(struct uci_context *ctx, char *path)
{
	char *file = basename(path);
	char *dir = dirname(path);
	char *err;
	struct uci_package *pkg;

	uci_set_confdir(ctx, dir);

	if (uci_load(ctx, file, &pkg)) {
		uci_get_errorstr(ctx, &err, file);
		free(err);
		return NULL;
	}

	return pkg;
}

enum {
	MOUNT_UUID,
	MOUNT_LABEL,
	MOUNT_ENABLE,
	MOUNT_TARGET,
	MOUNT_DEVICE,
	MOUNT_OPTIONS,
	__MOUNT_MAX
};

static struct blob_buf b;

int SDCardIcon::UciDetectSdcard(char *cfg)
{
	struct uci_context *ctx = uci_alloc_context();
	struct uci_package *pkg = NULL;
	struct uci_element *e;
	char path[64];
	blobmsg_policy mount_policy[__MOUNT_MAX];
	uci_blob_param_list mount_attr_list;

	mount_policy[MOUNT_UUID].name = "uuid";
	mount_policy[MOUNT_UUID].type = BLOBMSG_TYPE_STRING;
	mount_policy[MOUNT_LABEL].name = "label";
	mount_policy[MOUNT_LABEL].type = BLOBMSG_TYPE_STRING;
	mount_policy[MOUNT_DEVICE].name = "device";
	mount_policy[MOUNT_DEVICE].type = BLOBMSG_TYPE_STRING;
	mount_policy[MOUNT_TARGET].name = "target";
	mount_policy[MOUNT_TARGET].type = BLOBMSG_TYPE_STRING;
	mount_policy[MOUNT_OPTIONS].name = "options";
	mount_policy[MOUNT_OPTIONS].type = BLOBMSG_TYPE_STRING;
	mount_policy[MOUNT_ENABLE].name = "enabled";
	mount_policy[MOUNT_ENABLE].type = BLOBMSG_TYPE_INT32;

	mount_attr_list.n_params = __MOUNT_MAX;
	mount_attr_list.params = mount_policy;

	if (cfg) {
		snprintf(path, sizeof(path), "%s/upper/etc/config/fstab", cfg);
		pkg = config_try_load(ctx, path);

		if (!pkg) {
			snprintf(path, sizeof(path), "%s/etc/config/fstab", cfg);
			pkg = config_try_load(ctx, path);
		}
	}

	if (!pkg) {
		snprintf(path, sizeof(path), "/etc/config/fstab");
		pkg = config_try_load(ctx, path);
	}

	if (!pkg) {
		ILOG_ERROR(MGUI_SDCARD, "no usable configuration\n");
		return 0;
	}

	uci_foreach_element(&pkg->sections, e) {
		struct uci_section *s = uci_to_section(e);

		if (!strcmp(s->type, "mount")) {
			struct blob_attr *tb[__MOUNT_MAX] = { 0 };

			blob_buf_init(&b, 0);
			uci_to_blob(&b, s, &mount_attr_list);
			blobmsg_parse(mount_policy, __MOUNT_MAX, tb, blob_data(b.head), blob_len(b.head));
			if (!tb[MOUNT_LABEL] && !tb[MOUNT_UUID] && !tb[MOUNT_DEVICE])
				return 0;

			if (tb[MOUNT_ENABLE] && !blobmsg_get_u32(tb[MOUNT_ENABLE]))
				return 0;
			return 1;
		}
	}
	return 0;
}
#else
int SDCardIcon::UciDetectSdcard(char *cfg)
{
    return 0;
}
#endif

SDCardIcon::SDCardIcon(ilixi::Widget* parent)
        : SimpleIcon(parent),
          _sdcardState(SDCardUnknown)
{
    if (UciDetectSdcard(0))
        setSdcardState(SDCardActive);
    else
        setSdcardState(SDCardMissing);
}

SDCardIcon::~SDCardIcon()
{
}

SDCardState
SDCardIcon::getSdcardState() const
{
    return _sdcardState;
}

void
SDCardIcon::setSdcardState(SDCardState sdcardState)
{
    if (_sdcardState != sdcardState)
    {
        _sdcardState = sdcardState;
        switch (_sdcardState)
        {
            case SDCardActive:
                setImage("sdcard");
                break;
            case SDCardMissing:
                setImage("sdcard_missing");
                break;
            default:
                break;
        }
    }
}

} /* namespace MGUI */
