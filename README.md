# MCU (ESP32) Lockup Detection Using Internal WatchDog
## I. Introduction
If your program gets stuck for some reason, the fastest way to recover is to restart your computer. One of the most common examples was when the Windows operating system would crash (experience a fatal error) and display the Blue Screen of Death (BSoD), after which the machine would reboot to avoid further damage. Unlike desktop computers, embedded devices must be self-reliant. What if you are unable to restart the computer physically? If the program hangs, it's not always possible to wait for anyone to reset them. Some embedded designs, such as space probes, are simply inaccessible to humans, and a manual reset is not feasible; these could become permanently disabled if they were unable to autonomously recover from faults. In such situations, a human operator's capacity to reset the device will be inadequate to satisfy the product's uptime requirements. As firmware engineers, it is our responsibility to ensure that our devices can always recover from a stuck task or infinite loop. A watchdog is the only way usually employed in cases like these [[1](https://www.microcontrollertips.com/whats-watch-dog-timer-wdt-faq/)], [[2](https://www.embedded.com/introduction-to-watchdog-timers/)]. Watchdog is typically used in embedded systems and other computer-controlled equipment where humans cannot easily access the equipment or would be unable to respond to faults promptly [[3](https://www.researchgate.net/publication/295010877_Architecture_and_Operation_of_a_Watchdog_Timer)].

## II. What is Watchdog Timer?
A **watchdog timer** (WDT) is a hardware timer that can be used to automatically detect software anomalies and automatically generates a system reset if the main program neglects to periodically service it. It is often used to automatically reset an embedded device that hangs because of a software or hardware fault [[4](https://os.mbed.com/cookbook/WatchDog-Timer)]. Generally speaking, a watchdog timer is based on a counter that counts down from some initial value to zero. The embedded software selects the counter's initial value and periodically restarts it. If the counter ever reaches zero before the software restarts it, the software is presumed to be malfunctioning and the processor's reset signal is asserted [[2](https://www.embedded.com/introduction-to-watchdog-timers/)]. The process of restarting the watchdog timer's counter is sometimes called “**kicking the dog**.” The appropriate visual metaphor is that of a man being attacked by a vicious dog. If he keeps kicking the dog, it can't ever bite him. But he must keep kicking the dog at regular intervals to avoid a bite. Similarly, the software must restart the watchdog timer at a regular rate, or risk being restarted. Another term for “*kicking*” is “*feeding*” the watchdog timer. The watchdog timer is continuously fed with the initial value so that the timer value can never reach zero.

| ![space-1.jpg](https://github.com/TronixLab/ESP32_Watchdog/blob/main/docx/1.jpg?raw=true) | 
|:--:| 
| **Fig. 1** *A typical Watchdog setup* |

Fig. 1 shows a typical arrangement of the watchdog timer as an external block to the processor. However, it could also be included within the same chip as the CPU. This is done in many microcontrollers. In either case, the output from the watchdog timer is tied directly to the processor's reset signal. 

**Listing 1** *Basic Watchdog Timer*

In some engineer's opinion that, if an MCU could fail, the same could happen to its internal WDT. Thus, the external WDT was their preferred option. Using an internal WDT requires one less component and is, therefore, cheaper [[6](https://www.eeweb.com/using-the-microcontrollers-watchdog-timer-wdt-effectively/)]. On the other hand, there’s still a valid argument to be made for using an external WDT, because an internal WDT can be turned off accidentally by runaway code. Ultimately, the choice of internal versus external WDT relies on the nature of the application and the production budget for the hardware.

## III. What are the potential causes of the hang event?
A Microcontroller Unit (MCU) that causes failure can cause a complete standstill in operations. This can inconvenience users or pose functional safety risks in critical applications. A safety mechanism is extremely important. A reliable embedded system requires a combined effort from the hardware designer and the firmware programmer. There are some hardware and software-related factors why an MCU might have a failure mechanism [[7](https://resources.altium.com/p/microcontroller-failure-modes-why-they-happen-and-how-to-prevent-them)].
* Electrical noise can cause a power glitch, it may corrupt the program counter.
* Unstable power source.
* Memory stack overflow.
* A hardware component is not responding to request.
* Electrical interference (back-EMF, EMI, electrostatic discharge, cosmic ray, etc.).
* Dead-lock condition.
* A lot of interrupts (e.g. interrupt service routine, timer interrupts, etc.).

As one of the example demonstrated by **Listing 2**, *Deadlock* is a phenomenon when two tasks are in a blocked state waiting for the resources held by one and another simultaneously. When using a real-time operating system (FreeRTOS), such a deadlock can only be avoided by carefully programming real-time applications. Unlike the priority assignment protocol, FreeRTOS does not have a solution to the issue of deadlock. It can only be solved when designing real-time embedded systems. We must design tasks in such a way that a deadlock does not occur. 

**Listing 2** *Deadlock*

## IV. Structure of Watchdog Timer
Kicking the dog at a regular interval proves that the software is running. It is often a good idea to kick the dog only if the system passes some sanity check, as shown in **Fig. 2**:  The main program typically has a loop that it constantly goes through performing various functions. The watchdog timer is loaded with an initial value greater than the worst-case time delay through the main program loop. Each time it goes through the main loop the code resets the watchdog timer   (“kicking” or “feeding” the dog). If a fault occurs and the main program does not get back to reset the timer before it counts down to zero, an interrupt is generated to reset the processor. Used in this way, the watchdog timer can detect a fault on an unattended Arduino program and attempt corrective action with a reset [[5](https://www.digikey.com/en/articles/a-designers-guide-to-watchdog-timers)]. The best example of this structure is shown in **Listing 1**.

| ![space-1.jpg](https://github.com/TronixLab/ESP32_Watchdog/blob/main/docx/2.jpg?raw=true) | 
|:--:| 
| **Fig. 2** *A simple sanity checking* |

In some cases, there are some buffers allocated or the status of some component may be checked before deciding to kick the dog. Good design of such checks will increase the family of errors that the watchdog will detect.  One approach is to clear some flags before each loop is started, as shown in **Fig. 3**.

| ![space-1.jpg](https://github.com/TronixLab/ESP32_Watchdog/blob/main/docx/3.jpg?raw=true) | 
|:--:| 
| **Fig. 3** *A sanity checking with multiple flags in a single loop* |

Each flag is set at a certain point in the loop. At the bottom of the loop, the dog is kicked, but first, the flags are checked to see that all of the important points in the loop have been visited. The structure is shown in Fig.3 can be demonstrated in **Listing 3**.

**Listing 3** *Sanity checking in a single loop*

For a structure requiring multitasking as shown in **Fig. 4**, particularly a system running on Real-Time Operating System (RTOS). This scheme uses a task dedicated to the watchdog as demonstrated in **Listing 4**. This task wakes up at a regular interval and checks the sanity of all other tasks in the system. If all tasks pass the test, the watchdog is kicked. The watchdog monitor task runs at a higher priority than the tasks it is monitoring.  

| ![space-1.jpg](https://github.com/TronixLab/ESP32_Watchdog/blob/main/docx/4.jpg?raw=true) | 
|:--:| 
| **Fig. 4** *A sanity checking with multiple flags in a single loop* |

**Listing 4** *Sanity checking with a multiple tasks*

The watchdog timeout can be chosen to be the maximum time during which all regular tasks have had a chance to run from their start point through one full loop back to their start point again. Each task has a flag which can have two values, **TRUE** and **FLASE**. The flag is later read and written by the monitor. The monitor's job is to wake up before the watchdog timeout expires and check the status of each flag. If all flags contain the value **TRUE**, every task got its turn to execute and the watchdog may be kicked. Some tasks may have executed several loops and set their flag to ALIVE several times, which is acceptable. After kicking the watchdog, the monitor sets all of the flags to **FLASE**. By the time the monitor task executes again, all of the **FLASE** flags should have been overwritten with **TRUE**. 

Since the WDT is the very last line of defense, its design must anticipate any failure mode. One may ask, “What are the characteristics of a great watchdog?”
* The WDT must be independent of the CPU
* The WDT must always, under any condition barring perhaps a hardware failure, bring the system back to life.
* Some WDTs issue a non-maskable interrupt (NMI) instead of a reset.

**Listing 5** *The Great Watchdog*
