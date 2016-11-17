# Libvirt state file
### Structure of state file

1. Qemu header
|Title|Data|
|---|---|
|Qemu magic|QEVM|
|Qemu version|0x 00 00 00 03|

2. Section tables
|Title|Data|
|---|---|
|Section start|0x 01|
|Section ID|0x 00 00 00 02|
|Length of device name|0x 03|
|Device name|ram|
|Instance ID|0x 00 00 00 00|
|Version ID|0x 00 00 00 04|
|Total memory size|0x 00 00 00 01 00 8d 20 04|
|Sub-sections||
|End of section|0x 00 00 00  00 00 00 00 10|

3. Sub-sections
|Title|Data|
|---|---|
|Length of sub-section name|0x 06|
|Sub-section name|pc.ram|
|memory size|0x 00 00 00 01 00 00 00 00|

|Title|Data|
|---|---|
|Length of sub-section name|0x 08|
|Sub-section name|vga.vram|
|memory size|0x 00 00 00 00 00 80 00 00|
...
|Title|Data|
|---|---|
|Length of sub-section name|0x 12|
|Sub-section name|/rom@etc/acpi/rsdp|
|memory size|0x 00 00 00 00 00 00 10 00|

4. Section parts
|Title|Data|
|---|---|
|Section part|0x 02|
|Section ID|0x 00 00 00 02|
|Content||

5. Content
* RAM_SAVE_FLAG_COMPRESS = 0x02
* RAM_SAVE_FLAG_PAGE     = 0x08
* RAM_SAVE_FLAG_CONTINUE = 0x20
* ex. Flag = 0x 00  00 00 00 00 00 30 22
  * 0x 30 00: 地址(0x 10 00是4K，所以這是第3頁的意思)
  * 0x 00 20: 接續前面的page
  * 0x 00 02: 壓縮模式

|Title|Data|
|---|---|
|Flag|0x 00 00 00 00 00 00 00 08|
|Length of sub-section name|0x 06|
|Sub-section name|pc.ram|
|Data|0x 53 ff 00 f0 53 ff 00 f0 c3 e2 00 f0 53 ff 00 f0 ...|

|Title|Data|
|---|---|
|Flag|0x 00 00 00 00 00 00 10 28|
|Data|0x 30 20 53 59 53 20 2f 66 6f 72 63 65 22 20 6f 72 ...|

|Title|Data|
|---|---|
|Flag|0x 00 00 00 00 00 00 20 28|
|Data|0x e9 3d 06 00 01 00 00 00 3f 00 18 20 00 00 00 00 ...|

|Title|Data|
|---|---|
|Flag|0x 00  00 00 00 00 00 30 22|
|Data|0x 00|

|Title|Data|
|---|---|
|Flag|0x 00 00 00 00 00 00 40 22|
|Data|0x 00|

### Reference
* [qemu live migration 流程](http://blog.chinaunix.net/uid-29634482-id-5175302.html)
* [libvirt savevm 命令存储内存文件分析](http://blog.csdn.net/ssdxiao0/article/details/52221644)
* [save file analysis](https://github.com/juergh/lqs2mem.py)
