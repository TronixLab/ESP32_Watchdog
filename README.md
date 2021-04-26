# MCU (ESP32) Lockup Detection Using Internal WatchDog
## I. Introduction
If your program gets stuck for some reason, the fastest way to recover is to restart your computer. One of the most common examples was when the Windows operating system would crash (experience a fatal error) and display the Blue Screen of Death (BSoD), after which the machine would reboot to avoid further damage. Unlike desktop computers, embedded devices must be self-reliant. What if you are unable to restart the computer physically? If the program hangs, it's not always possible to wait for anyone to reset them. Some embedded designs, such as space probes, are simply inaccessible to humans, and a manual reset is not feasible; these could become permanently disabled if they were unable to autonomously recover from faults. In such situations, a human operator's capacity to reset the device will be inadequate to satisfy the product's uptime requirements. As firmware engineers, it is our responsibility to ensure that our devices can always recover from a stuck task or infinite loop. A watchdog is the only way usually employed in cases like these [[1](https://www.microcontrollertips.com/whats-watch-dog-timer-wdt-faq/)], [[2](https://www.embedded.com/introduction-to-watchdog-timers/)]. Watchdog is typically used in embedded systems and other computer-controlled equipment where humans cannot easily access the equipment or would be unable to respond to faults promptly [[3](https://www.researchgate.net/publication/295010877_Architecture_and_Operation_of_a_Watchdog_Timer)].
## II. What is Watchdog Timer?
A **watchdog timer** (WDT) is a hardware timer that can be used to automatically detect software anomalies and automatically generates a system reset if the main program neglects to periodically service it. It is often used to automatically reset an embedded device that hangs because of a software or hardware fault [[4](https://os.mbed.com/cookbook/WatchDog-Timer)]. Generally speaking, a watchdog timer is based on a counter that counts down from some initial value to zero. The embedded software selects the counter's initial value and periodically restarts it. If the counter ever reaches zero before the software restarts it, the software is presumed to be malfunctioning and the processor's reset signal is asserted [[2](https://www.embedded.com/introduction-to-watchdog-timers/)]. The process of restarting the watchdog timer's counter is sometimes called “**kicking the dog**.” The appropriate visual metaphor is that of a man being attacked by a vicious dog. If he keeps kicking the dog, it can't ever bite him. But he must keep kicking the dog at regular intervals to avoid a bite. Similarly, the software must restart the watchdog timer at a regular rate, or risk being restarted. Another term for “*kicking*” is “*feeding*” the watchdog timer. The watchdog timer is continuously fed with the initial value so that the timer value can never reach zero.
