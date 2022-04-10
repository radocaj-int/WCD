# WCD
Windbg companion driver

U need Visual Studio 2022 to build WCD Build debug configuration.

Disable Secure boot on machine where u plan to load driver. Set up testsigning mode by:

bcdedit /set testsigning on

Load driver by:

sc create WCD type= kernel binPath= c:\Test\WCD.sys sc start WCD

unload driver by: sc stop WCD

WCDClient [-r] read access operation 
          [-w] write access operation
          [-p] <threadid> <priority>
