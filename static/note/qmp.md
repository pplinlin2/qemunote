# QMP 
### Basic Usage
* __Q__emu Monitor Protocol
* 從virsh執行:
```
[~] # virsh qemu-monitor-command ${VM_NAME} --pretty '{"execute":"query-version"}'
{
    "return": {
        "qemu": {
            "micro": 1,
            "minor": 3,
            "major": 2
        },
        "package": ""
    },
    "id": "libvirt-98"
}
```
### Hello world
* 在`qapi-schema.json`中加入`{ 'command': 'hello-world' }`
* 在`qmp.c`中加入
```
void qmp_hello_world(Error **errp)
{
    printf("Hello world!^ ^\n");
}
```
* 在`qmp-commands.hx`加入
```
    {
        .name       = "hello-world",
        .args_type  = "",
        .mhandler.cmd_new = qmp_marshal_input_hello_world,
    },
```
* 執行`virsh qemu-monitor-command ${VM_NAME} --pretty '{"execute":"hello-world"}'`後後可在log檔看到`Hello world!^ ^`
### Argument
* 在`qapi-schema.json`中加入`{ 'command': 'hello-world', 'data': { '*message': 'str' } }`
  * *號代表這個欄位是optional
* 在`qmp.c`中加入
  * optional欄位都會有一個以has_開頭的boolean參數
  * c參數的順序必須跟schema中定義的一樣
```
void qmp_hello_world(bool has_message, const char *message, Error **errp)
{
    if (has_message) {
        printf("%s\n", message);
    } else {
        printf("Hello world!^ ^\n");
    }
}
```
* 在`qmp-commands.hx`加入
  * args_type中的s代表string, ?代表optional
```
    {
        .name       = "hello-world",
        .args_type  = "message:s?",
        .mhandler.cmd_new = qmp_marshal_input_hello_world,
    },
```
* 執行`virsh qemu-monitor-command ${VM_NAME} --pretty '{"execute":"hello-world"}'`後後可在log檔看到`Hello world!^ ^`
* 執行`virsh qemu-monitor-command ${VM_NAME} --pretty '{"execute":"hello-world", "arguments": { "message": "good job" }}'`後後可在log檔看到`good job`
### Errors
* 在`qmp.c`加入
```
void qmp_hello_world(bool has_message, const char *message, Error **errp)
{
    if (has_message) {
        if (strstr(message, "orz")) {
            error_set(errp, ERROR_CLASS_GENERIC_ERROR,
                      "the word 'orz' is not allowed");
            return;
        }
        printf("%s\n", message);
    } else {
        printf("Hello world!^ ^\n");
    }
}
```
* 執行時回傳錯誤
```
[~] # virsh qemu-monitor-command ${VM_NAME} --pretty '{"execute":"hello-world", "arguments": { "message": "orzzz" }}'
{
    "id": "libvirt-188",
    "error": {
        "class": "GenericError",
        "desc": "the word 'orz' is not allowed"
    }
}
```
# HMP
### Basic Usage
* Human Monitor Protocol
* Help: `virsh qemu-monitor-command ${VM_NAME} --hmp "help"`
### Hello World
* 在`hmp.c`加入
```
void hmp_hello_world(Monitor *mon, const QDict *qdict)
{
    const char *message = qdict_get_try_str(qdict, "message");
    Error *err = NULL;

    qmp_hello_world(!!message, message, &err);
    if (err) {
        monitor_printf(mon, "%s\n", error_get_pretty(err));
        error_free(err);
        return;
    }
}
```
* `hmp.h`中加入`void hmp_hello_world(Monitor *mon, const QDict *qdict);`
* `hmp-command.hx`中在`@end table`前加入
```
    {
        .name       = "hello-world",
        .args_type  = "message:s?",
        .params     = "hello-world [message]",
        .help       = "Print message to the standard output",
        .mhandler.cmd = hmp_hello_world,
    },

STEXI
@item hello_world @var{message}
@findex hello_world
Print message to the standard output
ETEXI
```
* 執行`virsh qemu-monitor-command ${VM_NAME} --hmp 'hello-world'`後後可在log檔看到Hello world!^ ^

### Reference
* qemu-src/docs/writing-qmp-commands.txt
