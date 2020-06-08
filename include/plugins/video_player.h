#ifndef _OMXPLAYER_VIDEO_PLAYER_PLUGIN_H
#define _OMXPLAYER_VIDEO_PLAYER_PLUGIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdatomic.h>

#include <pthread.h>
#include <systemd/sd-bus.h>

#include <collection.h>

#define LOAD_LIBSYSTEMD_PROC(name) \
    do { \
        libsystemd.name = dlsym(libsystemd.handle, #name); \
        if (!libsystemd.name) {\
            perror("could not resolve dbus procedure " #name); \
            return errno; \
        } \
    } while (false)

#define LOAD_LIBSYSTEMD_PROC_OPTIONAL(name) \
    libsystemd.name = dlsym(libsystemd.handle, #name)

#define DBUS_OMXPLAYER_OBJECT "/org/mpris/MediaPlayer2"
#define DBUS_OMXPLAYER_PLAYER_FACE "org.mpris.MediaPlayer2.Player"
#define DBUS_OMXPLAYER_ROOT_FACE "org.mpris.MediaPlayer2.Root"
#define DBUS_PROPERTY_FACE "org.freedesktop.DBus.Properties"
#define DBUS_PROPERTY_GET "Get"
#define DBUS_PROPRETY_SET "Set"

struct libsystemd {
	void *handle;

	int (*sd_bus_default)(sd_bus **ret);
	int (*sd_bus_default_user)(sd_bus **ret);
	int (*sd_bus_default_system)(sd_bus **ret);

	int (*sd_bus_open)(sd_bus **ret);
	int (*sd_bus_open_with_description)(sd_bus **ret, const char *description);
	int (*sd_bus_open_user)(sd_bus **ret);
	int (*sd_bus_open_user_with_description)(sd_bus **ret, const char *description);
	int (*sd_bus_open_system)(sd_bus **ret);
	int (*sd_bus_open_system_with_description)(sd_bus **ret, const char *description);
	int (*sd_bus_open_system_remote)(sd_bus **ret, const char *host);
	int (*sd_bus_open_system_machine)(sd_bus **ret, const char *machine);

	int (*sd_bus_new)(sd_bus **ret);

	int (*sd_bus_set_address)(sd_bus *bus, const char *address);
	int (*sd_bus_set_fd)(sd_bus *bus, int input_fd, int output_fd);
	int (*sd_bus_set_exec)(sd_bus *bus, const char *path, char *const argv[]);
	int (*sd_bus_get_address)(sd_bus *bus, const char **address);
	int (*sd_bus_set_bus_client)(sd_bus *bus, int b);
	int (*sd_bus_is_bus_client)(sd_bus *bus);
	int (*sd_bus_set_server)(sd_bus *bus, int b, sd_id128_t bus_id);
	int (*sd_bus_is_server)(sd_bus *bus);
	int (*sd_bus_set_anonymous)(sd_bus *bus, int b);
	int (*sd_bus_is_anonymous)(sd_bus *bus);
	int (*sd_bus_set_trusted)(sd_bus *bus, int b);
	int (*sd_bus_is_trusted)(sd_bus *bus);
	int (*sd_bus_set_monitor)(sd_bus *bus, int b);
	int (*sd_bus_is_monitor)(sd_bus *bus);
	int (*sd_bus_set_description)(sd_bus *bus, const char *description);
	int (*sd_bus_get_description)(sd_bus *bus, const char **description);
	int (*sd_bus_negotiate_creds)(sd_bus *bus, int b, uint64_t creds_mask);
	int (*sd_bus_negotiate_timestamp)(sd_bus *bus, int b);
	int (*sd_bus_negotiate_fds)(sd_bus *bus, int b);
	int (*sd_bus_can_send)(sd_bus *bus, char type);
	int (*sd_bus_get_creds_mask)(sd_bus *bus, uint64_t *creds_mask);
	int (*sd_bus_set_allow_interactive_authorization)(sd_bus *bus, int b);
	int (*sd_bus_get_allow_interactive_authorization)(sd_bus *bus);
	int (*sd_bus_set_exit_on_disconnect)(sd_bus *bus, int b);
	int (*sd_bus_get_exit_on_disconnect)(sd_bus *bus);
	int (*sd_bus_set_close_on_exit)(sd_bus *bus, int b);
	int (*sd_bus_get_close_on_exit)(sd_bus *bus);
	int (*sd_bus_set_watch_bind)(sd_bus *bus, int b);
	int (*sd_bus_get_watch_bind)(sd_bus *bus);
	int (*sd_bus_set_connected_signal)(sd_bus *bus, int b);
	int (*sd_bus_get_connected_signal)(sd_bus *bus);
	int (*sd_bus_set_sender)(sd_bus *bus, const char *sender);
	int (*sd_bus_get_sender)(sd_bus *bus, const char **ret);

	int (*sd_bus_start)(sd_bus *bus);

	int (*sd_bus_try_close)(sd_bus *bus);
	void (*sd_bus_close)(sd_bus *bus);

	sd_bus *(*sd_bus_ref)(sd_bus *bus);
	sd_bus *(*sd_bus_unref)(sd_bus *bus);
	sd_bus *(*sd_bus_close_unref)(sd_bus *bus);
	sd_bus *(*sd_bus_flush_close_unref)(sd_bus *bus);

	void (*sd_bus_default_flush_close)(void);

	int (*sd_bus_is_open)(sd_bus *bus);
	int (*sd_bus_is_ready)(sd_bus *bus);

	int (*sd_bus_get_bus_id)(sd_bus *bus, sd_id128_t *id);
	int (*sd_bus_get_scope)(sd_bus *bus, const char **scope);
	int (*sd_bus_get_tid)(sd_bus *bus, pid_t *tid);
	int (*sd_bus_get_owner_creds)(sd_bus *bus, uint64_t creds_mask, sd_bus_creds **ret);

	int (*sd_bus_send)(sd_bus *bus, sd_bus_message *m, uint64_t *cookie);
	int (*sd_bus_send_to)(sd_bus *bus, sd_bus_message *m, const char *destination, uint64_t *cookie);
	int (*sd_bus_call)(sd_bus *bus, sd_bus_message *m, uint64_t usec, sd_bus_error *ret_error, sd_bus_message **reply);
	int (*sd_bus_call_async)(sd_bus *bus, sd_bus_slot **slot, sd_bus_message *m, sd_bus_message_handler_t callback, void *userdata, uint64_t usec);

	int (*sd_bus_get_fd)(sd_bus *bus);
	int (*sd_bus_get_events)(sd_bus *bus);
	int (*sd_bus_get_timeout)(sd_bus *bus, uint64_t *timeout_usec);
	int (*sd_bus_process)(sd_bus *bus, sd_bus_message **r);
	int (*sd_bus_process_priority)(sd_bus *bus, int64_t max_priority, sd_bus_message **r);
	int (*sd_bus_wait)(sd_bus *bus, uint64_t timeout_usec);
	int (*sd_bus_flush)(sd_bus *bus);

	sd_bus_slot* (*sd_bus_get_current_slot)(sd_bus *bus);
	sd_bus_message* (*sd_bus_get_current_message)(sd_bus *bus);
	sd_bus_message_handler_t (*sd_bus_get_current_handler)(sd_bus *bus);
	void* (*sd_bus_get_current_userdata)(sd_bus *bus);

	int (*sd_bus_attach_event)(sd_bus *bus, sd_event *e, int priority);
	int (*sd_bus_detach_event)(sd_bus *bus);
	sd_event *(*sd_bus_get_event)(sd_bus *bus);

	int (*sd_bus_get_n_queued_read)(sd_bus *bus, uint64_t *ret);
	int (*sd_bus_get_n_queued_write)(sd_bus *bus, uint64_t *ret);

	int (*sd_bus_set_method_call_timeout)(sd_bus *bus, uint64_t usec);
	int (*sd_bus_get_method_call_timeout)(sd_bus *bus, uint64_t *ret);

	int (*sd_bus_add_filter)(sd_bus *bus, sd_bus_slot **slot, sd_bus_message_handler_t callback, void *userdata);
	int (*sd_bus_add_match)(sd_bus *bus, sd_bus_slot **slot, const char *match, sd_bus_message_handler_t callback, void *userdata);
	int (*sd_bus_add_match_async)(sd_bus *bus, sd_bus_slot **slot, const char *match, sd_bus_message_handler_t callback, sd_bus_message_handler_t install_callback, void *userdata);
	int (*sd_bus_add_object)(sd_bus *bus, sd_bus_slot **slot, const char *path, sd_bus_message_handler_t callback, void *userdata);
	int (*sd_bus_add_fallback)(sd_bus *bus, sd_bus_slot **slot, const char *prefix, sd_bus_message_handler_t callback, void *userdata);
	int (*sd_bus_add_object_vtable)(sd_bus *bus, sd_bus_slot **slot, const char *path, const char *interface, const sd_bus_vtable *vtable, void *userdata);
	int (*sd_bus_add_fallback_vtable)(sd_bus *bus, sd_bus_slot **slot, const char *prefix, const char *interface, const sd_bus_vtable *vtable, sd_bus_object_find_t find, void *userdata);
	int (*sd_bus_add_node_enumerator)(sd_bus *bus, sd_bus_slot **slot, const char *path, sd_bus_node_enumerator_t callback, void *userdata);
	int (*sd_bus_add_object_manager)(sd_bus *bus, sd_bus_slot **slot, const char *path);

	sd_bus_slot* (*sd_bus_slot_ref)(sd_bus_slot *slot);
	sd_bus_slot* (*sd_bus_slot_unref)(sd_bus_slot *slot);

	sd_bus* (*sd_bus_slot_get_bus)(sd_bus_slot *slot);
	void *(*sd_bus_slot_get_userdata)(sd_bus_slot *slot);
	void *(*sd_bus_slot_set_userdata)(sd_bus_slot *slot, void *userdata);
	int (*sd_bus_slot_set_description)(sd_bus_slot *slot, const char *description);
	int (*sd_bus_slot_get_description)(sd_bus_slot *slot, const char **description);
	int (*sd_bus_slot_get_floating)(sd_bus_slot *slot);
	int (*sd_bus_slot_set_floating)(sd_bus_slot *slot, int b);
	int (*sd_bus_slot_set_destroy_callback)(sd_bus_slot *s, sd_bus_destroy_t callback);
	int (*sd_bus_slot_get_destroy_callback)(sd_bus_slot *s, sd_bus_destroy_t *callback);

	sd_bus_message* (*sd_bus_slot_get_current_message)(sd_bus_slot *slot);
	sd_bus_message_handler_t (*sd_bus_slot_get_current_handler)(sd_bus_slot *slot);
	void *(*sd_bus_slot_get_current_userdata)(sd_bus_slot *slot);

	int (*sd_bus_message_new)(sd_bus *bus, sd_bus_message **m, uint8_t type);
	int (*sd_bus_message_new_signal)(sd_bus *bus, sd_bus_message **m, const char *path, const char *interface, const char *member);
	int (*sd_bus_message_new_method_call)(sd_bus *bus, sd_bus_message **m, const char *destination, const char *path, const char *interface, const char *member);
	int (*sd_bus_message_new_method_return)(sd_bus_message *call, sd_bus_message **m);
	int (*sd_bus_message_new_method_error)(sd_bus_message *call, sd_bus_message **m, const sd_bus_error *e);
	int (*sd_bus_message_new_method_errorf)(sd_bus_message *call, sd_bus_message **m, const char *name, const char *format, ...) _sd_printf_(4, 5);
	int (*sd_bus_message_new_method_errno)(sd_bus_message *call, sd_bus_message **m, int error, const sd_bus_error *e);
	int (*sd_bus_message_new_method_errnof)(sd_bus_message *call, sd_bus_message **m, int error, const char *format, ...) _sd_printf_(4, 5);

	sd_bus_message* (*sd_bus_message_ref)(sd_bus_message *m);
	sd_bus_message* (*sd_bus_message_unref)(sd_bus_message *m);

	int (*sd_bus_message_seal)(sd_bus_message *m, uint64_t cookie, uint64_t timeout_usec);

	int (*sd_bus_message_get_type)(sd_bus_message *m, uint8_t *type);
	int (*sd_bus_message_get_cookie)(sd_bus_message *m, uint64_t *cookie);
	int (*sd_bus_message_get_reply_cookie)(sd_bus_message *m, uint64_t *cookie);
	int (*sd_bus_message_get_priority)(sd_bus_message *m, int64_t *priority);

	int (*sd_bus_message_get_expect_reply)(sd_bus_message *m);
	int (*sd_bus_message_get_auto_start)(sd_bus_message *m);
	int (*sd_bus_message_get_allow_interactive_authorization)(sd_bus_message *m);

	const char *(*sd_bus_message_get_signature)(sd_bus_message *m, int complete);
	const char *(*sd_bus_message_get_path)(sd_bus_message *m);
	const char *(*sd_bus_message_get_interface)(sd_bus_message *m);
	const char *(*sd_bus_message_get_member)(sd_bus_message *m);
	const char *(*sd_bus_message_get_destination)(sd_bus_message *m);
	const char *(*sd_bus_message_get_sender)(sd_bus_message *m);
	const sd_bus_error *(*sd_bus_message_get_error)(sd_bus_message *m);
	int (*sd_bus_message_get_errno)(sd_bus_message *m);

	int (*sd_bus_message_get_monotonic_usec)(sd_bus_message *m, uint64_t *usec);
	int (*sd_bus_message_get_realtime_usec)(sd_bus_message *m, uint64_t *usec);
	int (*sd_bus_message_get_seqnum)(sd_bus_message *m, uint64_t* seqnum);

	sd_bus* (*sd_bus_message_get_bus)(sd_bus_message *m);
	sd_bus_creds *(*sd_bus_message_get_creds)(sd_bus_message *m); /* do not unref the result */

	int (*sd_bus_message_is_signal)(sd_bus_message *m, const char *interface, const char *member);
	int (*sd_bus_message_is_method_call)(sd_bus_message *m, const char *interface, const char *member);
	int (*sd_bus_message_is_method_error)(sd_bus_message *m, const char *name);
	int (*sd_bus_message_is_empty)(sd_bus_message *m);
	int (*sd_bus_message_has_signature)(sd_bus_message *m, const char *signature);

	int (*sd_bus_message_set_expect_reply)(sd_bus_message *m, int b);
	int (*sd_bus_message_set_auto_start)(sd_bus_message *m, int b);
	int (*sd_bus_message_set_allow_interactive_authorization)(sd_bus_message *m, int b);

	int (*sd_bus_message_set_destination)(sd_bus_message *m, const char *destination);
	int (*sd_bus_message_set_sender)(sd_bus_message *m, const char *sender);
	int (*sd_bus_message_set_priority)(sd_bus_message *m, int64_t priority);

	int (*sd_bus_message_append)(sd_bus_message *m, const char *types, ...);
	int (*sd_bus_message_appendv)(sd_bus_message *m, const char *types, va_list ap);
	int (*sd_bus_message_append_basic)(sd_bus_message *m, char type, const void *p);
	int (*sd_bus_message_append_array)(sd_bus_message *m, char type, const void *ptr, size_t size);
	int (*sd_bus_message_append_array_space)(sd_bus_message *m, char type, size_t size, void **ptr);
	int (*sd_bus_message_append_array_iovec)(sd_bus_message *m, char type, const struct iovec *iov, unsigned n);
	int (*sd_bus_message_append_array_memfd)(sd_bus_message *m, char type, int memfd, uint64_t offset, uint64_t size);
	int (*sd_bus_message_append_string_space)(sd_bus_message *m, size_t size, char **s);
	int (*sd_bus_message_append_string_iovec)(sd_bus_message *m, const struct iovec *iov, unsigned n);
	int (*sd_bus_message_append_string_memfd)(sd_bus_message *m, int memfd, uint64_t offset, uint64_t size);
	int (*sd_bus_message_append_strv)(sd_bus_message *m, char **l);
	int (*sd_bus_message_open_container)(sd_bus_message *m, char type, const char *contents);
	int (*sd_bus_message_close_container)(sd_bus_message *m);
	int (*sd_bus_message_copy)(sd_bus_message *m, sd_bus_message *source, int all);

	int (*sd_bus_message_read)(sd_bus_message *m, const char *types, ...);
	int (*sd_bus_message_readv)(sd_bus_message *m, const char *types, va_list ap);
	int (*sd_bus_message_read_basic)(sd_bus_message *m, char type, void *p);
	int (*sd_bus_message_read_array)(sd_bus_message *m, char type, const void **ptr, size_t *size);
	int (*sd_bus_message_read_strv)(sd_bus_message *m, char ***l); /* free the result! */
	int (*sd_bus_message_skip)(sd_bus_message *m, const char *types);
	int (*sd_bus_message_enter_container)(sd_bus_message *m, char type, const char *contents);
	int (*sd_bus_message_exit_container)(sd_bus_message *m);
	int (*sd_bus_message_peek_type)(sd_bus_message *m, char *type, const char **contents);
	int (*sd_bus_message_verify_type)(sd_bus_message *m, char type, const char *contents);
	int (*sd_bus_message_at_end)(sd_bus_message *m, int complete);
	int (*sd_bus_message_rewind)(sd_bus_message *m, int complete);

	int (*sd_bus_get_unique_name)(sd_bus *bus, const char **unique);
	int (*sd_bus_request_name)(sd_bus *bus, const char *name, uint64_t flags);
	int (*sd_bus_request_name_async)(sd_bus *bus, sd_bus_slot **ret_slot, const char *name, uint64_t flags, sd_bus_message_handler_t callback, void *userdata);
	int (*sd_bus_release_name)(sd_bus *bus, const char *name);
	int (*sd_bus_release_name_async)(sd_bus *bus, sd_bus_slot **ret_slot, const char *name, sd_bus_message_handler_t callback, void *userdata);
	int (*sd_bus_list_names)(sd_bus *bus, char ***acquired, char ***activatable); /* free the results */
	int (*sd_bus_get_name_creds)(sd_bus *bus, const char *name, uint64_t mask, sd_bus_creds **creds); /* unref the result! */
	int (*sd_bus_get_name_machine_id)(sd_bus *bus, const char *name, sd_id128_t *machine);

	int (*sd_bus_call_method)(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *ret_error, sd_bus_message **reply, const char *types, ...);
	int (*sd_bus_call_method_async)(sd_bus *bus, sd_bus_slot **slot, const char *destination, const char *path, const char *interface, const char *member, sd_bus_message_handler_t callback, void *userdata, const char *types, ...);
	int (*sd_bus_get_property)(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *ret_error, sd_bus_message **reply, const char *type);
	int (*sd_bus_get_property_trivial)(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *ret_error, char type, void *ret_ptr);
	int (*sd_bus_get_property_string)(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *ret_error, char **ret); /* free the result! */
	int (*sd_bus_get_property_strv)(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *ret_error, char ***ret); /* free the result! */
	int (*sd_bus_set_property)(sd_bus *bus, const char *destination, const char *path, const char *interface, const char *member, sd_bus_error *ret_error, const char *type, ...);

	int (*sd_bus_reply_method_return)(sd_bus_message *call, const char *types, ...);
	int (*sd_bus_reply_method_error)(sd_bus_message *call, const sd_bus_error *e);
	int (*sd_bus_reply_method_errorf)(sd_bus_message *call, const char *name, const char *format, ...) _sd_printf_(3, 4);
	int (*sd_bus_reply_method_errno)(sd_bus_message *call, int error, const sd_bus_error *e);
	int (*sd_bus_reply_method_errnof)(sd_bus_message *call, int error, const char *format, ...) _sd_printf_(3, 4);

	int (*sd_bus_emit_signal)(sd_bus *bus, const char *path, const char *interface, const char *member, const char *types, ...);

	int (*sd_bus_emit_properties_changed_strv)(sd_bus *bus, const char *path, const char *interface, char **names);
	int (*sd_bus_emit_properties_changed)(sd_bus *bus, const char *path, const char *interface, const char *name, ...) _sd_sentinel_;

	int (*sd_bus_emit_object_added)(sd_bus *bus, const char *path);
	int (*sd_bus_emit_object_removed)(sd_bus *bus, const char *path);
	int (*sd_bus_emit_interfaces_added_strv)(sd_bus *bus, const char *path, char **interfaces);
	int (*sd_bus_emit_interfaces_added)(sd_bus *bus, const char *path, const char *interface, ...) _sd_sentinel_;
	int (*sd_bus_emit_interfaces_removed_strv)(sd_bus *bus, const char *path, char **interfaces);
	int (*sd_bus_emit_interfaces_removed)(sd_bus *bus, const char *path, const char *interface, ...) _sd_sentinel_;

	int (*sd_bus_query_sender_creds)(sd_bus_message *call, uint64_t mask, sd_bus_creds **creds);
	int (*sd_bus_query_sender_privilege)(sd_bus_message *call, int capability);

	int (*sd_bus_match_signal)(sd_bus *bus, sd_bus_slot **ret, const char *sender, const char *path, const char *interface, const char *member, sd_bus_message_handler_t callback, void *userdata);
	int (*sd_bus_match_signal_async)(sd_bus *bus, sd_bus_slot **ret, const char *sender, const char *path, const char *interface, const char *member, sd_bus_message_handler_t match_callback, sd_bus_message_handler_t add_callback, void *userdata);

	int (*sd_bus_creds_new_from_pid)(sd_bus_creds **ret, pid_t pid, uint64_t creds_mask);
	sd_bus_creds *(*sd_bus_creds_ref)(sd_bus_creds *c);
	sd_bus_creds *(*sd_bus_creds_unref)(sd_bus_creds *c);
	uint64_t (*sd_bus_creds_get_mask)(const sd_bus_creds *c);
	uint64_t (*sd_bus_creds_get_augmented_mask)(const sd_bus_creds *c);

	int (*sd_bus_creds_get_pid)(sd_bus_creds *c, pid_t *pid);
	int (*sd_bus_creds_get_ppid)(sd_bus_creds *c, pid_t *ppid);
	int (*sd_bus_creds_get_tid)(sd_bus_creds *c, pid_t *tid);
	int (*sd_bus_creds_get_uid)(sd_bus_creds *c, uid_t *uid);
	int (*sd_bus_creds_get_euid)(sd_bus_creds *c, uid_t *euid);
	int (*sd_bus_creds_get_suid)(sd_bus_creds *c, uid_t *suid);
	int (*sd_bus_creds_get_fsuid)(sd_bus_creds *c, uid_t *fsuid);
	int (*sd_bus_creds_get_gid)(sd_bus_creds *c, gid_t *gid);
	int (*sd_bus_creds_get_egid)(sd_bus_creds *c, gid_t *egid);
	int (*sd_bus_creds_get_sgid)(sd_bus_creds *c, gid_t *sgid);
	int (*sd_bus_creds_get_fsgid)(sd_bus_creds *c, gid_t *fsgid);
	int (*sd_bus_creds_get_supplementary_gids)(sd_bus_creds *c, const gid_t **gids);
	int (*sd_bus_creds_get_comm)(sd_bus_creds *c, const char **comm);
	int (*sd_bus_creds_get_tid_comm)(sd_bus_creds *c, const char **comm);
	int (*sd_bus_creds_get_exe)(sd_bus_creds *c, const char **exe);
	int (*sd_bus_creds_get_cmdline)(sd_bus_creds *c, char ***cmdline);
	int (*sd_bus_creds_get_cgroup)(sd_bus_creds *c, const char **cgroup);
	int (*sd_bus_creds_get_unit)(sd_bus_creds *c, const char **unit);
	int (*sd_bus_creds_get_slice)(sd_bus_creds *c, const char **slice);
	int (*sd_bus_creds_get_user_unit)(sd_bus_creds *c, const char **unit);
	int (*sd_bus_creds_get_user_slice)(sd_bus_creds *c, const char **slice);
	int (*sd_bus_creds_get_session)(sd_bus_creds *c, const char **session);
	int (*sd_bus_creds_get_owner_uid)(sd_bus_creds *c, uid_t *uid);
	int (*sd_bus_creds_has_effective_cap)(sd_bus_creds *c, int capability);
	int (*sd_bus_creds_has_permitted_cap)(sd_bus_creds *c, int capability);
	int (*sd_bus_creds_has_inheritable_cap)(sd_bus_creds *c, int capability);
	int (*sd_bus_creds_has_bounding_cap)(sd_bus_creds *c, int capability);
	int (*sd_bus_creds_get_selinux_context)(sd_bus_creds *c, const char **context);
	int (*sd_bus_creds_get_audit_session_id)(sd_bus_creds *c, uint32_t *sessionid);
	int (*sd_bus_creds_get_audit_login_uid)(sd_bus_creds *c, uid_t *loginuid);
	int (*sd_bus_creds_get_tty)(sd_bus_creds *c, const char **tty);
	int (*sd_bus_creds_get_unique_name)(sd_bus_creds *c, const char **name);
	int (*sd_bus_creds_get_well_known_names)(sd_bus_creds *c, char ***names);
	int (*sd_bus_creds_get_description)(sd_bus_creds *c, const char **name);

	void (*sd_bus_error_free)(sd_bus_error *e);
	int (*sd_bus_error_set)(sd_bus_error *e, const char *name, const char *message);
	int (*sd_bus_error_setf)(sd_bus_error *e, const char *name, const char *format, ...)  _sd_printf_(3, 4);
	int (*sd_bus_error_set_const)(sd_bus_error *e, const char *name, const char *message);
	int (*sd_bus_error_set_errno)(sd_bus_error *e, int error);
	int (*sd_bus_error_set_errnof)(sd_bus_error *e, int error, const char *format, ...) _sd_printf_(3, 4);
	int (*sd_bus_error_set_errnofv)(sd_bus_error *e, int error, const char *format, va_list ap) _sd_printf_(3,0);
	int (*sd_bus_error_get_errno)(const sd_bus_error *e);
	int (*sd_bus_error_copy)(sd_bus_error *dest, const sd_bus_error *e);
	int (*sd_bus_error_move)(sd_bus_error *dest, sd_bus_error *e);
	int (*sd_bus_error_is_set)(const sd_bus_error *e);
	int (*sd_bus_error_has_name)(const sd_bus_error *e, const char *name);

	int (*sd_bus_error_add_map)(const sd_bus_error_map *map);

	int (*sd_bus_path_encode)(const char *prefix, const char *external_id, char **ret_path);
	int (*sd_bus_path_encode_many)(char **out, const char *path_template, ...);
	int (*sd_bus_path_decode)(const char *path, const char *prefix, char **ret_external_id);
	int (*sd_bus_path_decode_many)(const char *path, const char *path_template, ...);

	int (*sd_bus_track_new)(sd_bus *bus, sd_bus_track **track, sd_bus_track_handler_t handler, void *userdata);
	sd_bus_track* (*sd_bus_track_ref)(sd_bus_track *track);
	sd_bus_track* (*sd_bus_track_unref)(sd_bus_track *track);

	sd_bus* (*sd_bus_track_get_bus)(sd_bus_track *track);
	void *(*sd_bus_track_get_userdata)(sd_bus_track *track);
	void *(*sd_bus_track_set_userdata)(sd_bus_track *track, void *userdata);

	int (*sd_bus_track_add_sender)(sd_bus_track *track, sd_bus_message *m);
	int (*sd_bus_track_remove_sender)(sd_bus_track *track, sd_bus_message *m);
	int (*sd_bus_track_add_name)(sd_bus_track *track, const char *name);
	int (*sd_bus_track_remove_name)(sd_bus_track *track, const char *name);

	int (*sd_bus_track_set_recursive)(sd_bus_track *track, int b);
	int (*sd_bus_track_get_recursive)(sd_bus_track *track);

	unsigned (*sd_bus_track_count)(sd_bus_track *track);
	int (*sd_bus_track_count_sender)(sd_bus_track *track, sd_bus_message *m);
	int (*sd_bus_track_count_name)(sd_bus_track *track, const char *name);

	const char* (*sd_bus_track_contains)(sd_bus_track *track, const char *name);
	const char* (*sd_bus_track_first)(sd_bus_track *track);
	const char* (*sd_bus_track_next)(sd_bus_track *track);

	int (*sd_bus_track_set_destroy_callback)(sd_bus_track *s, sd_bus_destroy_t callback);
	int (*sd_bus_track_get_destroy_callback)(sd_bus_track *s, sd_bus_destroy_t *ret);
};

struct omxplayer_mgr;

struct omxplayer_video_player {
	char 	event_channel_name[256];
	char    video_uri[256];
	int64_t player_id;
	int64_t view_id;

	/// If flutter says the video should be looping, this value is true.
	/// Note: this is not related to whether omxplayer is looping.
	/// omxplayer is always looping so it doesn't accidentally terminate on us.
	bool    should_loop;

	struct omxplayer_mgr *mgr;
};

struct omxplayer_mgr {
	pthread_t thread;
	struct omxplayer_video_player *player;
	struct concurrent_queue task_queue;
};

enum omxplayer_mgr_task_type {
	kCreate,
	kDispose,
	kListen,
	kUnlisten,
	kSetLooping,
	kSetVolume,
	kPlay,
	kPause,
	kGetPosition,
	kSetPosition,
	kUpdateView
};

struct omxplayer_mgr_task {
	enum omxplayer_mgr_task_type type;

	FlutterPlatformMessageResponseHandle *responsehandle;
	
	union {
		struct {
			char *omxplayer_dbus_name;
			bool omxplayer_online;
		};
		bool loop;
		float volume;
		int64_t position;
		struct {
			bool visible;
		};
		struct {
			int offset_x, offset_y;
			int width, height;
			int zpos;
		};
	};
};



#define OMXPLAYER_VIDEO_PLAYER_INITIALIZER {0}

enum data_source_type {
	kDataSourceTypeAsset,
	kDataSourceTypeNetwork,
	kDataSourceTypeFile
};

extern int omxpvidpp_init(void);
extern int omxpvidpp_deinit(void);

#endif