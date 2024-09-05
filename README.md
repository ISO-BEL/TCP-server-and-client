# Why Im Learning
* Learning this because future projects i have require understanding networking and the TCP protocol.
* My next project uses the HTTP protocol which as a protocol is built on top of TCP (execpt for http/3.0 which uses UDP and QUIC).
* Its enjoyable to learn

## Goals:
* understand socket api.
* understand the basic system calls.
* understand different protocol layers (wrapped in protol).
* understand comminication with the kernal (signals/system calls/buffer).
* understand osi model.
* understand networks on a large scale (WAN, LAN, etc).
* understand socket states.
* understand blocking/polling 
* understand signals
* IP version agnostic.
* concurrent (fork'ed).
* signals to prevent zombie processes.
* handle signals causing system calls to interupt.
* error handling for every system call.
* preventing zombie processes from simultaneous same signals.
* insure all of the message is sent and recv'ed (is byte stream).

### Possibly An Improvement
* use the sigpromask() function to catch signals/clean up zombies only in specific parts of the code.
* 


--Error Handling Resource
* https://www.microfocus.com/documentation/server-enterprise-edition/serveree60sp2ws2/Server%20EE%2060%20SP2%20WS2%20UNIX=1=Web%20Help%202016%20(MF)=en/GUID-1872DF9A-0FE4-4093-9A1B-B743BFDDDBA1.html


