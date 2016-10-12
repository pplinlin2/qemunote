# Qemu source analysis
### trace 使用方式
* qemu編譯時: `./configure ... --enable-trace-backends=simple`
* qemu: 
`qemu-system-x86_64 ... -trace events=/root/trace.events,file=/root/trace.log`
* libvirt: 在xml中加入
```
<domain type='kvm' xmlns:qemu='http://libvirt.org/schemas/domain/qemu/1.0'>
  <qemu:commandline>
    <qemu:arg value='-trace'/>
    <qemu:arg value='events=/root/trace.events,file=/root/trace.log'/>
  </qemu:commandline>
</domain>
```
* 將要追蹤的函式加入trace.events之中
```
[~] # cat /root/trace.events
vnc_key_guest_leds
vnc_key_sync_numlock
vnc_key_sync_capslock
vnc_key_map_init
vnc_key_event_ext
vnc_key_event_map
```
* 由qemu源碼中的simpletrace.py來分析log檔trace.log
```
[~/qemu-2.3.1] # ./scripts/simpletrace.py trace-events trace.log
vnc_key_event_map 0.000 pid=14002 down=0x1 sym=0xffe1 keycode=0x2a name=shift
vnc_key_event_map 318459.976 pid=14002 down=0x1 sym=0x21 keycode=0x2 name=1
vnc_key_event_map 147880.165 pid=14002 down=0x0 sym=0x21 keycode=0x2 name=1
vnc_key_event_map 808690.485 pid=14002 down=0x1 sym=0x3f keycode=0x35 name=slash
vnc_key_event_map 111673.133 pid=14002 down=0x0 sym=0x3f keycode=0x35 name=slash
vnc_key_event_map 439906.877 pid=14002 down=0x0 sym=0xffe1 keycode=0x2a name=shift
```
### VNC keyboard input
* qemu執行時建立起keysym2keycode陣列
* 〔ui/keymaps.c〕
```
init_keyboard_layout
  parse_keyboard_layout
    分析pc-bios/keymaps/en-us
    add_keysym
```
* 以en-us的編碼為例的keysym2keycode陣列
```
Setting keysym 1 (0x31) to 0x2
Setting keysym 2 (0x32) to 0x3
Setting keysym 3 (0x33) to 0x4
Setting keysym 4 (0x34) to 0x5
Setting keysym 5 (0x35) to 0x6
Setting keysym 6 (0x36) to 0x7
Setting keysym 7 (0x37) to 0x8
Setting keysym 8 (0x38) to 0x9
Setting keysym 9 (0x39) to 0xa
Setting keysym 0 (0x30) to 0xb
Setting keysym q (0x71) to 0x10
Setting keysym Q (0x51) to 0x110
...
Setting keysym at (0x40) to 0x103
‧‧‧
```
* [ui/vnc.c]
```
protocol_client_msg
  key_event
    do_key_event
```
* 可以在wireshark中filter輸入vnc.key_down來偵測封包

|message-type(4)|down-flag(1)|padding(2)|keysym(4)  |
|---------------|------------|----------|-----------|
|             04|          01|     00 00|00 00 00 40|
|      Key Event|         Yes|          |          @|
* 分析收到的vnc封包「@」，由keysym2keycode得到keycode
  * keysym: @ (0x40)
  * keycode: 0x103


### Reference
 * [key injection](https://www.berrange.com/posts/2011/09/23/injecting-fake-keyboard-events-to-kvm-guests-via-libvirt/)
 * [key codes](https://www.berrange.com/tags/key-codes/)

