What's New?
===========
Scroll down a bit to the section "QP/C Revision History".

Installing QP/C
===============
QP/C can be unzipped anywhere in your file system, but the recommended
location is C:\qp\qpc on Windows and ~/qp/qpc on Linux/MacOS-X.

The Generally Available QP/C release contains the QP/C source code
(independent on any particular processor or compiler), detailed "QP/C
Reference Manual" in CHM, and a tutorial that explains step-by-step how
to build a non-trivial real-time application with QP.

Also, the Generally Available release of QP/C contains ports and
examples for ARM Cortex-M as well as ports and examples for desktop
systems (Windows, Linux/POSIX), which you can run without any embedded
targets.


Defining the QPC Environment Variable
=====================================
The project files and Makefiles provided in the "examples" directory
assume that the environment variable QPC is defined and it points to the
location of the QP/C framework. For example, assuming that you have
installed QP/C into the directory C:\qp\qpc, you should define the
environment variable QPC to C:\qp\qpc.

Additionally, the provided Makefiles for win32\mingw (Windows with MinGW
GNU-based compiler) assume that the MinGW toolset is available. The
MinGW toolset is now part of the Qtools collection for Windows, which
should be unzipped to your disk and added to the PATH. For example,
assuming that you have the Qtools collection into the C:\tools\qtools\
directory, please make sure that your PATH contains C:\tools\qtools\bin.

Editing the Environment Variables on Windows
--------------------------------------------
Open the Windows Explorer and right-click on the Computer icon. Then
select Properties from the pop-up menu. This opens a window with the
basic information about your computer. In the left panel of this window,
click on "Advanced system settings". This opens the dialog "System
Properties". Click the "Environment Variables..." button.

To add a new environment variable, click the "New..." button in the
System Variables section and provide the variable name (e.g., QPC) and
value (e.g., C:\software\qpc).

To modify the Path variable, look in the bottom panel "System Variables"
and scroll down to find the variable Path. Click on it and then press
"Edit..." button. Click on the Variable value edit field. At the end of
the string append ;C:\tools\qtools\bin (assuming that you have installed
the Qtools collection in C:\tools\qtools). Make sure that there is a
semicolon ';' between the last entry and the added path.

Editing the Environment Variables on Linux and Mac OS X
-------------------------------------------------------
You need to edit the ~/.bash_profile file (~/.profile in some systems)
to add the following lines:

export QPC=~/qp/qpc
export QTOOLS=~/tools/qtools

assuming that you have installed QP/C into ~/qp/qpc and Qtools into
~/tools/qtools.


QP/C Development Kits (QDKs)
============================
The QP/C Baseline Code usually needs to be adapted to various operating
systems, microprocessors, and compilers. Adapting the QP software is
called porting and QP has been designed from ground up to make the
porting easy.

A large and steadily growing number of QP/C Development Kits (QDKs) for
popular processors (such as ARM7/ARM9, Atmel AVR and AVR32, TI MSP430,
TI TMS320C28x, TMS320C55x, Renesas RX600, M16C/R8C, Microchip
PIC24/dsPIC, PIC32, FreeScale ColdFire) and operating systems  (such as
Linux/BSD, Windows/WinCE, VxWorks, ThreadX, FreeRTOS.org, uC/OS-II,
eCos) are available for download from:

http://www.state-machine.com/downloads/

Please keep checking this website, as new QDKs are added frequently.

All QDKs are designed to "plug-into" the directory structure already
established after the installation of the QP/C baseline code. In other
words, you need to install the QP baseline code, before you install any QDK.


Contact information:
====================
Quantum Leaps Websites: http://www.state-machine.com
                        http://www.quantum-leaps.com
e-mail:                 info@quantum-leaps.com
toll-free:              1-866-450-LEAP (US Eastern Standard Time)

#######################################################################
#################### QP/C Revision History ############################

QP/C Version 5.1.1 (10-Oct-2013)
--------------------------------
This release fixes reversal of logic in the QF_noTimeEvtsActiveX()
function as well as sleep mode transition in the ARM Cortex-M3/M4 ports
to the cooperative Vanilla kernel. Also, the native QP event queue
implementation has been changed to count the extra "front-event"
location into the number of free entries, which fixes the problem of
defer queues of depth 1. Finally, the release restores the support for
linting (with PC-Lint) of the QP/C applications for ARM Cortex-M (with
IAR and GNU compilers).

Changes in detail:

1. In file qf_tick.c reversed the logic inside QF_noTimeEvtsActiveX()

2. Modified free entry accounting (nFree) in the files: qeq_init.c,
qeq_fifo.c, qeq_get.c, and qeq_lifo.c.

3. Modified free entry accounting (nFree) in the files: qa_init.c,
qa_fifo.c, qa_get_.c, and qa_lifo.c.

4. Introduced new macro QF_CPU_SLEEP() in the ARM Cortex-M Vanilla ports.

5. Changed Board Support Package files (bsp.c) in the ARM Cortex-M
Vanilla examples.

6. Modified the CMSIS-compliant startup code in all ARM Cortex-M Vanilla
examples.

7. Modified the application examples with PC-Lint
(qpc\examples\arm-cm\qk\gnu\dpp-qk_ek-lm3s811-lint and
qpc\examples\arm-cm\qk\iar\dpp-qk_ek-lm3s811-lint). Updated lint files
for the latest PC-Lint


QP/C Version 5.1.0 (23-Sep-2013)
--------------------------------
This release brings significant improvements to the QS software tracing
implementation and also brings important changes the ARM Cortex-M port.

*********************
NOTE: QP/C 5.1.0 requires changing the interrupt priority setting in the
existing ARM Cortex-M applications. Specifically, you need to set the
interrupt priorities equal or lower than QF_AWARE_ISR_CMSIS_PRI constant
provided in the qf_port.h header file.
*********************

Changes to the QS software tracing component in detail:

1. Optimized the internal QS implementation of all functions that insert
trace data into the trace buffer. The general idea of the optimization
is to extensively use automatic variables instead of global variables
(such as buffer head and tail indexes, the running checksum, etc.). For
the modern CPUs (such as ARM) this resulting machine code performs most
operations in registers, instead of constantly updating the memory
through the expensive load/store instructions. The time savings through
avoiding load/store instructions are significant, even after taking the
performance hit from loading the registers from the globals in the
beginning of each function and storing the final register values into
the globals at the end.

2. Reduced the QS code size by using loops instead of unrolled-loops as
before. This reduced the QS component size from over 4KB to 1.7KB (for
ARM Cortex-M3/M4, IAR compiler).

3. Modified the make scripts for building QP libraries to use
higher-level optimization for the QS software tracing functions in the
SPY build configuration. This brings additional 20-50% speed
improvement, depending on the compiler and optimization options used.
Please note that only the QS component is built with high-optimization.
The QEP, QF, and QK components in the SPY configuration are still built
with low-optimization level, so that the application can be conveniently
debugged.

4. Reduced the number of QS global filters from 256 to 124 (0x7C). This
enables the code to avoid escaping the trace record numbers (because
they cannot overlap the special flag byte 0x7E or the escape byte 0x7D)
and also speeds up the QS_filterOff(QS_ALL_RECORDS) function, which is
useful for stopping the trace quickly to avoid overwriting some
interesting data with the new data.

5. An empty QS record and the QS_RESET record are now inserted
automatically into the trace buffer in the function QS_initBuf(). The
empty QS record/QS_RESET pair provides a clean start of a session and
allows the QSPY host application to re-synch with the data stream, even
if the last QS record of a previous session is incomplete. This ability
is very helpful for re-setting the target while collecting a trace.

Overall, lab tests for ARM Cortex-M4 with the IAR compiler show that the
processing time of the QS_u32_() function (the one frequently used to
store pointers and timestamps) dropped from 233 CPU cycles for QP 5.0
with low-level optimization to just 76 cycles for QP 5.1 with high-level
of optimization.  At the same time, the code size of this function
dropped from 876 bytes to 274 bytes.

Changes to the QP ports to ARM Cortex-M in detail:

QP 5.1.0 never completely disables interrupts in the ARM Cortex-M3/M4
cores, even inside the critical sections. On Cortex-M3/M4 (ARMv7-M
architectures), the QP port disables interrupts selectively using the
BASEPRI register. (NOTE: The BASEPRI register is not implemented in the
ARMv6-M architecture (Cortex-M0/M0+), so Cortex-M0/M0+ need to use the
PRIMASK register to disable interrupts globally).

This new policy of disabling interrupts divides interrupts into
"kernel-unaware" interrupts, which are never disabled, and
"kernel-aware" interrupts, which are disabled in the QP critical
sections. Only "kernel-aware" interrupts are allowed to call QP
services. "Kernel-unaware" interrupts are *NOT* allowed to call any QP
services and they can communicate with QP only by triggering a
"kernel-aware" interrupt (which can post or publish events).

As mentioned above, all QP ports to ARM Cortex-M included in the QP
5.1.0 Baseline Code provide the constant QF_AWARE_ISR_CMSIS_PRI, which
must be used to offset the "kernel-aware" interrupt priorities.

All example projects for ARM Cortex-M included in the QP 5.1.0 Baseline
Code demonstrate the recommended way of assigning interrupt priorities
in your applications. The initialization consist of two steps:

1. you enumerate the "kernel-unaware" and "kernel-aware" interrupt
priorities (whereas you offset the "kernel-aware" priorities by the
constant QF_AWARE_ISR_CMSIS_PRI) and

2. you assign the priorities to *ALL* interrupts by calling the
NVIC_SetPriority() CMSIS function.

****
NOTE: Leaving the interrupt priority at the default value of zero
(the highest priority) is most likely incorrect, because the
"kernel-unaware" interrupts cannot call any QP services.
****

For more information, please read the short Application Note "Setting
ARM Cortex-M Interrupt Priorities in QP 5.1" available at:
http://www.state-machine.com/arm/AN_ARM-Cortex-M_Interrupt-Priorities.pdf


QP/C Version 5.0.0 (10-Sep-2013)
--------------------------------

*********************
NOTE: QP/C 5.0.0 remains backwards-compatible with the existing QP/C 4.x
applications.
*********************

The main purpose of this milestone QP/C release is to enable the QM
modeling tool to generate a new type of state machine code (requires QM
version 3.0.0, which is still in development as of this writing).

This new type of state machine implementation in QP/C 5 is based on the
new QMsm class, which takes advantage of the QM tool as an advanced
"state machine compiler". QM can perform optimizations that were not
possible with the C pre-processor alone. Specifically, the QM can easily
determine the LCA (Least-Common-Ancestor) state for every transition and
it can generate the complete transition-sequences (sequences of
exit/entry/initial actions) at code-generation time. The resulting code
can be still highly human-readable, but it will no longer be
human-maintainable. The lab tests indicate that the new "housekeeping"
code for executing hierarchical state machines can be about twice as
fast as the previous code based on the QHsm class. Additionally, the new
code requires less run-time support (smaller event processor) and uses
70% less of stack space in the call to the QMsm_dispatch() operation
than QHsm_dispatch().

The next big feature introduced in QP/C 5 is polymorphism ("virtual"
functions) for basic operations, such as state machine init() and
dispatch() and active object start(), post(), and postLIFO() perations.
Making theses functions "virtual" means that all these operations can be
re-defined in sub-classes of state machines and active objects. This, in
turn, allows a single application to use a mix of state machine classes
derived from the new QMsm base class with state machines derived from
the QHsm base class, each one using a different state machine
implementation strategy. Additionally, the virtual post() operation
could be very useful for implementing various Proxy active objects
(e.g., for active object event posting across networks).

Another big feature introduced in QP/C 5 are the multiple, independent
system clock tick rates for time events. The number of system tick rates
can be now configured in the QP/C ports. For example, a digital watch
can use a "fast" clock tick rate of 100Hz and a "slow" clock tick rate
of only 1Hz. These clock tick rates can be managed independently, so for
example, the fast clock tick rate can be shut down in the absence of
time events assigned to this rate. This feature allows the applications
to implement sophisticated power-saving policies.

As yet another important feature, QP/C adds a new "extended" API for
non-asserting event allocation and posting. This feature is intended for
situations, where an application is hammered with external events that
at times arrive too fast for processing, but that can be ignored under
the overload conditions. In those cases firing an assertion inside the
framework is undesirable. The non-asserting API allows a designer to
request a safety-margin when allocating or posting an event. The event
is not allocated/posted if the safety margin cannot be satisfied at the
time of the call. On the other hand, the safety margin allows the
application to still use the regular (asserting) event allocation and
posting, because the event pools and event queues are guaranteed to
maintain a minimal margin for safe operation.

Finally, QP/C adds a number of smaller features and improvements,
summarized in the following detailed list of changes:

1. Added the new QMsm "class" to qep.h. Changed the inheritance tree by
deriving QHsm and QFsm from the QMsm base class. Added virtual table
structures for QMsm, QHsm, and QFsm (polymorphism).
    - added macro QMSM_INIT() to polymorphically call the state
      machine initialization implementation in the QMsm base class and
      all subclasses.
    - added macro QMSM_DISPATCH() to polymorphically call the state
      machine event dispatching implementation  in the QMsm base class
      and all subclasses.

2. Added new source files qmsm_ini.c and qmsm_dis.c to the QEP. These
files implement the QMsm_init() and QMsm_dispatch() functions, respectively.

3. Added the new "QMActive" "class" to qf.h. Extended the inheritance
tree to derive QMActive from QActive. Added virtual table structures for
QMActive and QActvie (polymorphism).
    - modified macro QACTIVE_POST() to polymorphically call the direct
      event posting to an active object.
    - modified macro QACTIVE_POST_LIFO() to polymorphically call the
      post-LIFO (self-posting) to an active object.
    - modified macro QACTIVE_START() to polymorphically call the
      starting of an active object.

4. Added the multiple system clock tick rates feature in qf.h:
    - added new configuration macro QF_MAX_TICK_RATE, which specifies
      the number of clock tick rates. This macro is to be defined in
      the QF ports (in the qf_port.h header file). If the macro is
      undefined, the default value is 1 (one clock tick rate).
    - renamed and re-implemented the QF_tick() function as the
      "extended" QF_tickX() function with the argument 'tickRate' for
      processing time events allocated to different clock rates. The
      application must call QF_tickX(0), QF_tickX(1), ... at the
      specified tick rates from ISRs or *tasks*.
    - added an "extended" time event constructor QTimeEvt_ctorX(), which
      assigns a time event to a specific tick rate as well as specific
      active object.
    - renamed and re-implemented the internal function QTimeEvt_arm_()
      to a public function QTimeEvt_armX() for arming time events
      initialized with the "extended" constructor. The QTimeEvt_armX()
      function is the new recommended API for arming time events, both
      one-shot and periodic.
    - re-implemented QTimeEvt_disarm() and QTimeEvt_rarm().
    - renamed QF_noTimeEvtsActive() to the "extended" version
      QF_noTimeEvtsActiveX(), which checks time events assigned to the
      given tick rate.

5. Added the new non-asserting API to qf.h:
    - renamed internal function QF_new_() to QF_newX_(), the latter one
      taking the argument 'margin' for allocating events. The function
      returns NULL if the event pool has less free events than the
      specified margin. The function asserts if the margin is zero and
      the event can't be allocated.
    - added function QActive_post() to post an event to the given
      active object. The function does not post the event if the target
      event queue has less free slots than the specified margin. The
      function asserts if the margin is zero and the event can't be
      posted.
    - added "extended" macro QF_NEW_X() for allocating events with a
      margin.
    - added "extended" macro QACTIVE_POST_X() for posting events with a
      margin.

6. Modified the QActive_defer() function to return the status of the
   defer operation (true==success), instead of asserting when the defer
   queue overflows.

7. Modified QS (Quantum Spy) software tracing implementation:
    - added additional tick rate byte to the trace records QS_QF_TICK
      and QS_QFF_TIMEEVT_*.
    - added new trace records QS_QF_ACTIVE_POST_ATTEMPT,
      QS_QF_EQUEUE_POST_ATTEMPT, and QS_QF_MPOOL_GET_ATTEMPT for the
      "extened" non-asserting event allocation and posting.
    - added new trace records QS_TEST_RUN and QS_TEST_FAIL for future
      support for unit testing.
    - added new QS source file qs_dict.c with functions QS_*_dict() to
      generate various dictionary entries. Changed the macros
      QS_*_DICTIONARY() to call these functions. This was done to
      significantly reduce the amount of tracing code needed to send the
      dictionaries from applications.
    - grouped together the various QS variables (such as filters, trace
      buffer indexes, etc.) in a single struct, which results in a more
      efficient code for various QS operations.

8. Changed the structure of the ARM Cortex-M ports
    - renamed the sub-directory for ARM Cortex-M ports and examples
      from "arm-cortex" to "arm-cm". This is done to avoid confusion
      with other ARM Cortex variants, such as Cortex-A/R, which very
      different from Cortex-M.
    - removed the CMSIS (Cortex Microcontroller Software Interface
      Standard) directories from the Cortex-M examples and moved it to
      the common location in the %QPC%\ports\arm-cm\cmsis\ directory.
      Upgraded the CMSIS to the latest version 3.20.
    - added the ARM Cortex-M ports and examples with Keil/ARM MDK to
      the QP Baseline Code.
    - upgraded ARM Cortex-M ports with IAR to the latest IAR EWARM 6.60
    - upgraded ARM Cortex-M ports with Sourcery CodeBench to the latest
      version 2013.05-53.

9. Added the requested simple "Blinky" example for Windows and ARM
Cortex-M (with the GNU, IAR, and Keil toolsets).
    - Added "Getting Started with QP/C" guide based on the Blinky
      example.

10. Updated the Doxygen documentation (QP/C Reference Manual)
    - updated the QP/C tutorial
    - updated and added documentation and code samples
    - added search box and tree-view panel to the HTML documentation


QP/C Version 4.5.04 (08-Feb-2013)
---------------------------------
The main purpose of this release is adding support for the ARM
Cortex-M4F processors with the hardware Floating-Point Unit (FPU). The
QP/C ports to Cortex-M4F take full advantage of the "lazy stacking"
feature of the FPU registers, and by doing so offer the most efficient
preemptive multitasking on this processor.

Changes in detail:

1. Added ports and examples for ARM Cortex-M4F with the EK-LM4F120XL
board (Stellaris Launchpad).

2. Added the macro QF_LOG2(), which can be re-implemented in the QP
ports, if the CPU supports special instructions, such as CLZ (count
leading zeros in Cortex-M3/M4). If the macro is not defined in the QP
port, the default implementation uses a lookup table.

3. Updated all ARM Cortex-M ports and examples to the latest IAR EWARM
6.50 and Sourcery CodeBench 2012.09-85.

4. Updated App Notes "QP and ARM Cortex-M with IAR" and "QP and ARM
Cortex-M with GNU".

5. Updated the PC-Lint support files (include\lib-qpc.lnt,
au-misra2.lnt) to the latest PC-Lint 9.00j.

6. Updated the Application Note: "QP/C MISRA-C:2004 Compliance Matrix".

7. Spell-checked the comments in all QP/C source files and removed
several typos.

8. Removed the Qt ports and examples from the QP/C Baseline Code and
moved them to the separate QDK/C-Qt.

***
NOTE: QP/C Version 4.5.04 preserves full compatibility with QM 2.2.03
and all existing QDKs for QP/C 4.5.xx.
***

QP/C Version 4.5.03 (28-Nov-2012)
---------------------------------
This release changes the directory structure of QP ports to various
operating systems, such as POSIX (Linux, BSD, etc.), Win32 (Windows),
Android-NDK, etc. The OS ports are moved from the ports\80x86\
directory one level up to ports\. Also, the OS examples are moved
from the examples\80x86\ directory one level up to examples\.


QP/C Version 4.5.02 (04-Aug-2012)
---------------------------------
The main purpose of this release is better, more comprehensive support
for (rapid) prototyping of embedded QP applications on the desktop with
the Win32 API and with Qt. Among others, this release adds a complete
toolkit for creating realistic embedded front panels with pure Win32-GUI
API and free Visual C++ Express and ResEdit. An extensive Application
Note "QP and Win32" is included in this release.

This release simplifies the QP ports to desktop OSs, such as Windows
(Win32), Linux, BSD, Mac OSX (POSIX) and combines 32-bit and 64-bit
ports in one with conditional compilation.

This release also adds an option for using constructors for dynamically
allocated events. When the configuration macro Q_EVT_CTOR is defined,
the Q_NEW() macro becomes variadic and takes the arguments for the event
constructor. This generally allows creating dynamic events "on-the-fly"
without a temporary pointer to the event. This QP configuration is
demonstrated only in the QP port to Qt, but can be used in any other port.

NOTE: The event constructor feature is NOT backward-compatible with the
existing applications.

This release also adds a new macro QF_MPOOL_EL, which is intended for
allocating properly aligned storage for memory pools and event pools.

All QP ports included in this release use only a single QP library,
rather than separate libraries for QEP, QF, QK, and QS.

Finally, this release adds QM models, created with the new QM 2.2.00
to most of the examples. The code generated by this new QM version
complies with MISRA-C:2004 rules.

Changes in detail:

1. Modified QP port to Win32 and used the free Visual C++ Express 2010
with Platform SDK rather than Visual C++ Pro 2008. Renamed the port
directory from vc2008\ to vc\. Provided a completely revised App Note
"QP and Win32".

2. Eliminated QP port to Win32 with one thread (Win32-1T).

3. Consolidated all QP ports to POSIX OSs (Linux, Linux64, Mac_OSX) into
a single port to POSIX and placed it in the directory posix\.

4. Renamed the port directory qt_1t\ to qt\.

5. Added event constructor to qevt.h (controlled by the configuration
macro Q_EVT_CTOR).

6. Added new variadic version of the Q_NEW() macro in qf.h when
Q_EVT_CTOR is defined.

7. Added macro QF_MPOOL_EL to qmpool.h. Modified all examples to
demonstrate the use of this macro to allocate properly aligned storage
for event pools.

8. Added new typedef 'enum_t' and modified signatures of functions
taking event signals from QSignal to enum_t. This was done to
significantly reduce the number of casts necessary when enumerated
signals were passed to QP functions.

9. Modified all QP ports distributed in the QP/C baseline code to
generate only a single QP library, rather than separate libraries for
QEP, QF, QK, and QS. This includes all QP ports to the desktop
(ports\80x86\ directory) and ARM Cortex-M ports (ports\arm-cortex\
directory).

10. Modified all examples to link only one QP library.

11. Added QM models to most examples and used the automatically
generated code from the models instead of the hand-written code.

12. Modified Qt ports to use the new "event constructors" and modified
examples for Qt to demonstrate this feature.

13. Added .ui files to the Qt examples for generating UIs graphically
with the Qt Designer tool. Revised and updated the App Note "QP and Qt".

14. Added new macro QS_USR_DICTIONARY() to QS for providing symbolic
names for user-defined trace records

15. Added new macro QS_RESET() to QS for telling the QSPY application
when the target resets. This allows QSPY to reset its internal state.


QP/C Version 4.5.01 (14-Jun-2012)
---------------------------------
The main purpose of this minor release is providing improved
MISRA-compliant state machine implementation. Specifically, a new macro
Q_UNHANDLED() has been added for a situation when a guard condition
evaluates to FALSE, but the state model does not prescribe the explicit
[else] case for this guard. In this case, the state handler can return
Q_UNHANDLED(), which will cause the QEP event processor to propagate the
event to the superstate, which is what UML semantics prescribes.

NOTE: These change to the QEP event processor is completely
backwards-compatible. All state handler functions coded the old way will
continue to handle the guard conditions correctly and in accordance with
the UML specification. The new Q_UNHANDLED() macro is necessary only for
MISRA-compliant state handler coding, which will be applied in the
upcoming release of the QM modeling and code generation tool.

Changes in detail:

1. Added macro Q_UNHANDLED() and return value Q_RET_UNHANDLED in qep.h.

2. Modified qhsm_dis.c to handle the Q_RET_UNHANDLED return value.

3. Updated the QP/C MISRA-C:2004 compliance matrix to include the new
MISRA-compliant way of coding guard conditions.

4. Modified qs.h and qs_dummy.h to add new trace record type
QS_QEP_UNHANDLED, which is generated when the state handler returns
Q_RET_UNHANDLED.

5. Modified qs.h and qs_dummy.h to add the User record dictionary trace
record and macro QS_USR_DICTIONARY().

NOTE: This new trace record requires the updated QSPY 4.5.01.

6. Corrected qfsm_dis.c, which did not generate QS trace records for
entry and exit from non-hierarchical states.

7. Updated the IAR ARM compiler used in the ARM Cortex-M examples to the
latest version IAR EWARM 6.40.

8. Modified the Qt port not to define the QPApp::onClockTick() slot
function, but instead to allow defining this slot function in the BSP of
the application.


QP/C Version 4.5.00 (29-May-2012)
---------------------------------
The main purpose of this release is to improve host-based development
of QP applications, which is critical for Test-Driven Development (TDD).
Among others, this release provides integration between QP and the
popular Qt GUI framework, which allows developers to easily build
host-based simulations of the embedded systems with the realistic user
interfaces.

This release also simplifies implementing transitions to history, which
is a preparation to providing this feature in the QM modeling tool.

Changes in detail:

1. Renamed the event class from QEvent to QEvt to avoid a name conflict
with the Qt framework. Also, for consistency, renamed the file qevent.h
to qevt.h and the macro Q_EVENT_CAST() to Q_EVT_CAST().

NOTE: To minimize impact of this change on the existing QP ports and
applications, the name QEvent is provided as well, but this can be
suppressed by defining the configuration macro Q_NQEVENT in qep_port.h.

2. Changed the design of QF_tick() (file qf_tick.c) to better support
calling this function from low-priority tasks (as opposed to interrupts
and highest-priority tasks), which often happens when QP is executed on
the desktop operating systems. In this design only QF_tick() can remove
time events from the active linked list, so no unexpected changes to the
list structure are eliminated.

3. Simplified the QTimeEvt class by removing the 'prev' link pointer,
as the new design no longer needs a bi-directional list. These changes
impact the files: qte_*.c.

4. Added return value to QF_run() to allow transfer of the exit
status to the destop operating systems.

NOTE: This modification haves impact on most QP/C ports, because
the QF_run() function must now return a int16_t value.

5. Eliminated the 'running' member of the QActive class, which
has been used only in the QP ports to "big" OSes such as Linux
or Windows.

6. Added member 'temp' to the QHsm and QFsm base classes to prevent
clobbering the current state (the 'state' member) during transitions.
This change allows keeping the current state unchanged during the
entire transition chain, which in turn allows easy and generic access
to the state information to store the state history in the exit
actions from states. Additional bonus of this re-design is the
opportunity of testing for stable state configuration in assertions
added to the qhsm_*.c and qfsm_*.c files.

7. Added the QHsm_state() and QFsm_state() accessor macros.

8. Modified the "Transition to History" pattern implementation to
use the simplified technique of obtaining the current state in the
exit action from the superstate rather than all the exit actions from
the substates. Modified the "State-Local Storage" (SLS) pattern as
well, because it was using the transition to history constructs.

9. Re-designed the implementation of the QSPY host application, so
that it can be conveniently included as part of the QP library.
This allows direct QS tracing output to the screen for QP applications
running on the desktop.

NOTE: This change is part of the Qtools release 4.5.00.

10. Modified the QP ports to Win32_1t (both the MinGW and VC2008) to
output QS trace data directly to the stdout via the QSPY host-application
interface. Modified the DPP examples for Win32_1T to demonstrate the
direct QS output to the screen.

11. Added QP port to Qt_1t (Qt with one thread) and two example
applications (DPP and PELICAN crossing).

12. Added GNU compiler option -pthread to QP ports for POSIX with
P-threads, including QP ports and examples for Linux and Mac OS X.


QP/C Version 4.4.01 (23-Mar-2012)
---------------------------------
The release fixes a bug in Q-SPY software tracing, which caused the
linking error: "Q_SIG_() not defined". This release also includes
a few cosmetic changes, which the Microchip C18 compiler didn't like.


1. Moved Q_SIG_() definition from qep.h to qs.h
2. Changed (QEvent *)0 to (QEvent const *)0 in source files
   qeq_get.c, qeq_init.c, and qa_get_.c.


QP/C Version 4.4.00 (31-Jan-2012)
---------------------------------
The main purpose of this release is MISRA-C:2004 compliance, strong-type
checking compliance, update of PC-Lint option files and tests, and
general cleanup.

1. Moved the qp_port.h header file from the port directories to
the qcp\include/ directory. Also, moved the inclusion of the
QS (Spy) header files (qs_port.h/qs_dummy.h) from qep.h, qf.h,
and qk.h headers to qp_port.h. These structural changes were made
to reduce the number of preprocessor #if nesting levels below 8,
which is the ANSI-C limit. This was done to comply with the MISRA-C
rule 1.1 (all code shall conform to ANSI/ISO C).

NOTE: This modifications have impact on most QP/C ports, because
the qp_port.h header file must be removed from the port.

2. Added the PC-Lint option files std.lnt and lib-qpc.lnt to the
qcp/include/ directory.

3. Cleaned the whole QP/C code from lint comments. All PC-Lint options
have been moved to PC-Lint option files.

4. Modified QP assertion macro Q_DEFINE_THIS_MODULE() to avoid using
the # operator (MISRA rule 19.13). This macro now requires the argument
enclosed in doble quotes "".

NOTE: This modification has impact on some QP/C ports.

4. Added typedefs for char_t, int_t, float32_t, and float64_t to
event.h header file for compliance with MISRA-C:2004 rules 6.1 and 6.3.

5. Added macros Q_STATE_CAST() and Q_EVENT_CAST() to qep.h to
encapsulate deviation from MISRA-C rule 11.4.

6. Added macro Q_UINT2PTR_CAST() to encapsulate casting unsigned
integers to pointers, which deviates from MISRA-C rule 11.3. This
macro has been added for *application-level* code.

7. Updated ARM Cortex-M examples with the latest CMSIS v3.0, which
complies with more MISRA-C:2004 rules.

8. Added DPP examples for MISRA-C:2004 compliant applications (for
IAR-ARM and GNU-ARM).

9. Added testing of PC-Lint option files against the MISRA-C Exemplar
Suite.

10. Updated ARM-Cortex-M3 port with GNU to the latest Sourcery
CodeBench 2011.09-60.

11. Added QP/C port to Win32-1t and examples (Windows with 1 thread).
This port is useful for testing embedded QP/C applications on windows.

12. Added documentation to QP/C distribution in the directory
qpc/doc/, with the following Application notes:
"MISRA-C:2004 Compliance Matrix", "Quantum Leaps Coding Standard",
"QP and ARM Cortex-M, and QP and Windows",


QP/C Version 4.3.00 (01-Nov-2011)
---------------------------------
1. This release changes the names of critical section macros and
introduces macros for unconditional interrupt disabling/enabling.
This is done to simplify and speed up the built-in Vanilla and QK
kernels, which no longer are dependent on the interrupt locking
policy.

NOTE: The change in handling the critical section in the Vanilla and
QK kernels can break QP ports, which use the "save and restore
interrupt lock" policy, because all such ports must also define
unconditional interrupt disabling and enabling.

2. This release changes the partitioning of the QK scheduler.
Specifically, the QK scheduler is now divided between two functions
QK_schedPrio_() and QK_sched_(), to calculate the highest-priority
task ready to run and to perform scheduling, respectively. The function
QK_schedPrio_() is useful to determine if scheduling is even necessary.

3. Updated all QP ports to comply with the new critical section
names and policies.

4. Modified the ARM Cortex-M port qk_port.s to take advantage of the
new structure of the QK scheduler.

5. Upgraded the examples for ARM Cortex with IAR EWARM to the
latest IAR EWARM version 6.30.

6. Upgraded the examples for ARM Cortex with GNU (CodeSourcery) to the
latest Sourcery CodeBench 2011.07-60.


QP/C Version 4.2.04 (24-Sep-2011)
---------------------------------
The main pupose of this relase is to provide a bug fix for the QK port
to ARM Cortex processors. The bug fix addresses a very rare and
undocumented behavior of late-arrival of an interrupt while entering
the PendSV exception. In this case the PENDSVSET bit in the NVIC-ICSR
register is *not* cleared when finally PendSV is entered, so the
PendSV exception is entered in a *different* state when it is entered
via the late-arrival mechanism versus the normal activation of the
exception through tail-chaining. The consequence of this undocumented
and inconsistent hardware behavior, PendSV could be re-entered again
before the SVCall exception cleans up the stack. The bug fix is
implemented in the qk_port.s file and consists of clearing the
PENDSVSET bit programmatically inside PendSV_Handler.


QP/C Version 4.2.02 (08-Sep-2012)
---------------------------------
1. The main pupose of this relase is to repackage the default QP/C
distribution to contain the single root directory qpc/ in the
archive. That way, unziping the archive will produce only one
directory (qpc/), which can be then changed by the user.

2. This release also changes the ARM Cortex QP ports with GNU. The
suffix "_cs" has been added to all QP libraries generated by the
Code Sourcery toolset (now Mentor Sourcery CodeBench). This is to
distinguish libraries generated by different GNU-toolchains (such
as CodeRed, Attolic, DevKit ARM, etc.)


QP/C Version 4.2.01 (13-Aug-2011)
---------------------------------
1. Modified file qassert.h to add assertion macros #Q_ASSERT_ID,
#Q_REQUIRE_ID, #Q_ENSURE_ID, #Q_INVARIANT_ID, and #Q_ERROR_ID,
which are better suited for unit testig, because they avoid the
volatility of line numbers for indentifying assertions.

2. Added QP port and examples for Mac OS X on 80x86.


QP/C Version 4.2.00 (14-Jan-2011)
---------------------------------
The goal of this milestone release is to extend the number of event
pools (theoretically up to 255 pools) instead of just three event
pools available up til now. Also, this release adds several
improvements to the QS/QSPY software tracing, such as adding sender
information to event posting so that sequence diagrams could be easily
and automatically reconstructed from tracing data. Also, the tracing
now supports 64-bit targets, such as embedded Linux 64-bit. Finally,
this milestone release migrates the examples to use the environment
variable QPC instead of relying on the relative path to the QP/C
framework. This allows easier adaptation of the examples for real
projects, which don't really belong to the examples directory.

The changes in detail are:

1. Changed the QEvent base struct (file qevent.h). The private member
'dynamic_' has been replaced by two members 'poolId_' and 'refCtr_'.

2. Added configuration macro QF_MAX_EPOOL (file qf.h) to define the
maximum number of event pools in the QP application (default to 3).
The maximum theoretical number of this macro is 255.

3. Made algorithmic changes in the QF source code related to the change
of storing the event pool-IDs and reference counters inside QEvent.

4. Changed the default signal size (macro Q_SIGNAL_SIZE in the file
qevent.h) from 1 to 2 bytes.

5. Changed the signature of QActive_recall() to return uint8_t instead
of QEvent*, which this could encourage incorrect usage (processing of
the event at the point of recalling it). Now, the function only returns
1 (TRUE) if the event was recalled and 0 (FALSE) if the event was not
recalled.

6. Added the function QTimeEvt_ctr() and new source file qte_ctr.c.
The function returns the counter of a time event, which allows
using a time event for measuring the time.

7. Added new QF macros #QF_TICK, #QF_PUBLISH, and #QACTIVE_POST in
file qf.h to provide sender of the events for software tracing.

8. Added new QS macros (files qs.h and qs_dummy.h) for handling 64-bit
integers.

9. Added the functions QS_u64() and QS_u64_() and new source file
qs_u64.c.

10. Added the QS macro #QS_U32_HEX_T for hexadecimal formatting of
integer numbers in the user-defined trace records.

11. Added the new port linux64 for 64-bit Linux. Also added the
corresponding examples for 64-bit Linux.

12. Adapted the QSPY host application for 64-bit pointer sizes and
the changed layout of trace records that contain event information
(such as PoolID and RefCtr). Also added the backwards-compatibility
option (-v) for switching the tool to the previous data format.

13. Removed the tools directory from the QPC distribution and moved
the QSPY host application to the QTOOLS distribution, which now also
contains the GNU make tools for Windows.

14. Modified the make files and project files to use the environment
variable QPC instead of relying on the relative path to the QP/C
framework.

15. Upgraded the examples for ARM Cortex with IAR EWARM to the
latest IAR EWARM 6.20.


QP/C Version 4.1.07 (28-Feb-2011)
---------------------------------
The goal of this release is to improve the ease of experimenting with
QP/C on the desktop. This release adds support for Windows (Win32) to
the baseline code. Two most popular compilers for Windows are supported:
Microsoft Visual Studio and MinGW (GNU). The support for Linux has been
improved by including pre-built QP/C libraries and improving makefiles
for Eclipse compatibility.

The changes in detail are:

1. Added Win32 port with the Visual C++ 2008 (ports/80x86/win32/vc2008).
This directory contains the Visual Studio solution all_qp.sln for
building all QP/C libraries from the IDE. Three build configurations
(Debug, Release, and Spy) are supported.

2. Added Win32 port with the MinGW comiler (ports/80x86/win32/mingw).
This directory contains the Makefile for building all QP/C libraries.
Three build configurations (dbg, rel, and spy) are supported.
NOTE: the Makefile assumes that the MinGW/bin directory is added
to the PATH.

3. Added Win32 examples for Visual C++ 2008 (examples/80x86/win32/
vc2008/dpp and examples/80x86/win32/vc2008/qhsmtst). Visual Studio
soultions are provides for all build configurations.

4. Added Win32 examples for MinGW (examples/80x86/win32/mingw/dpp
and examples/80x86/win32/mingw/qhsmtst). Eclipse-compatible makefiles
are provided for all build configurations. NOTE: the Makefiles assume
that the MinGW/bin directory is added to the PATH.

5. Removed memory alignment correction in the file qmp_init.c. This
correction required casting of pointers to integers and was problematic
on 64-bit targets (such as 64-bit Linux).

6. Upgraded the examples for ARM Cortex with CodeSourcery to the
latest Sourcery G++ 2011.02-2.


QP/C Version 4.1.06 (07-Jan-2011)
---------------------------------
1. Made cosmetic improvements to the example QM models of the
"Fly 'n' Shoot" game.

2. Made improvements in make.bat files for building the examples for
DOS/Open Watcom to run better in DosBox on Linux.

3. Upgraded the examples for ARM Cortex with IAR to the latest
IAR EWARM version 6.10.

4. Upgraded the examples for ARM Cortex with CodeSourcery to the
latest Sourcery G++ 2010.09-66.


QP/C Version 4.1.05 (01-Nov-2010)
---------------------------------
This release is adds examples for the QM (QP Modeler) graphical modeling
and code generation tool. The examples are based on the "Fly 'n' Shoot"
game described in the QP/C Tutorial and in Chapter 1 of the PSiCC2 book.

Specifically, the directory <qpc>\examples\80x86\dos\watcom\l\game-qm\
contains the "Fly 'n' Shoot" game model file "game.qm". This model, when
opened in the QM tool contains all state machine diagrams and generates
code into the subdirectory qm_code\. This code can then be built and
executed on any 80x86 machine (newer versions of Windows or Linux
require the DOSbox application, see http://www.dosbox.com).

The directory <qpc>\examples\arm-cortex\vanilla\iar\game-ev-lm3s811-qm\
contains the version of the game for the EV-LM3S811 ARM Cortex-M3 board.
This directory contains the model file "game.qm", which is actually
identical as the model in the DOS version. The LM3S811 version needs to
be compiled with the IAR compiler and executed on the EV-LM3S811 board.

Additionally, the QP/C baseline code has been slighlty modified for
better conformance to the MISRA C 2004 rules and the latest PC-Lint 9.x.


QP/C Version 4.1.04 (16-Mar-2010)
---------------------------------
This release is adds compatibility of all examples for DOS with the
DOSBox emulator (http://www.dosbox.com/) that recreates a MS-DOS
compatible environment on all versions of Windows, including 64-bit
Windows that don't run 16-bit DOS applications anymore.

Also, this release includes QP ports and examples for EV-LM3S811 board
with the GNU-based Code Sourcery G++ toolset. Support for Sourcery G++
provides a very cost-effective option for developing QP applications for
ARM Cortex MCUs.

Finally, this release improves the Cortex Microcontroller Software
Interface Standard (CMSIS) for the whole family of the Stellaris LM3Sxxx
MCUs. The improvement extends the CMSIS from Sandstorm to Fury,
DustDevil, and Tempest Stellaris families.


QP/C Version 4.1.03 (21-Feb-2010)
---------------------------------
This release is concerned with the ARM Cortex ports and examples.
Specifically, this release contains the following improvements:

1. Unified source code for ARM Cortex-M3 and the new ARM Cortex-M0
cores, including the code for the preemptive QK kernel.

2. Compliance with the Cortex Microcontroller Software Interface
Standard (CMSIS) in all ARM Cortex examples.

3. Backward-compatible support for the new LM3S811 EVAL Rev C board with
different OLED display than previous revisions. (NOTE: The OSRAM 96x16x1
OLED used in REV A-B boards has been replaced RITEK 96x16x1 OLED used in
Rev C.)

In the process of making the examples CMSIS-compliant, the dependency on
the Luminary Micro driver library (driverlib.a) has been completely removed.

Additionally, the screen saver of the "Fly 'n' Shoot" game has been
improved to periodically switch off the power of the OLED display, which
better protects the display from burn-in. The affected file is tunnel.c.

Finally, this release introduces the QP_VERSION macro, which identifies
the QP version. Otherwise, this maintenance release does not change the
QP/C API in any way, so the release has NO IMPACT on the QP/C
applications except for the ARM Cortex ports and applications.


QP/C Version 4.1.02 (14-Jan-2010)
---------------------------------
The purpose of this minor maintenance release is the change in the
directory structure for the ARM Cortex ports and examples. As new ARM
Cortex cores are becoming available, the old port name "cortex-m3" could
be misleading, because it actually applies to wider range of ARM Cortex
cores. Consequently, all ARM Cortex ports and examples are hosted in the
directory tree called "arm-cortex".

This maintenance release does not change the QP/C API in any way, so the
release has NO IMPACT on the QP/C applications except for the ARM Cortex
ports and applications.


QP/C Version 4.1.01 (05-Nov-2009)
---------------------------------
The main purpose of this release is to replace the Turbo C++ 1.01
toolset with the Open Watcom C/C++ toolset, because Turbo C++ 1.01 is no
longer available for a free download. In contrast, Open Watcom is
distributed under an OSI-certified open source license, which permits
free commercial and non-commercial use. Open Watcom can be downloaded
from www.openwatcom.org.

All 80x86/DOS, 80x86/qk, and 80x86/ucos2 ports and examples for Turbo
C++ 1.01 have been replaced with ports and examples for Open Watcom. The
make.bat scripts are provided to build the QP/C libraries and examples.

In the process of converting the examples to Open Watcom two new
examples have been added to the standard QP/C distribution. The Calc2
example located in <qpc>\examples\80x86\dos\watcom\l\calc2 shows how to
derive state machine classes with QP 4.x. The SLS example located in
<qpc>\examples\80x86\dos\watcom\l\sls shows the implemenation of the
new State-Local Storage state design pattern.


QP/C Version 4.1.00 (10-Oct-2009)
---------------------------------
The release provides brings a number of improvements to QP/C and updates
the QP/C ARM Cortex-M3 examples for the EK-LM3S811 board to the latest
IAR EWARM 5.40.

This maintenance release does not change the QP/C API in any way, so the
release has NO IMPACT on the QP/C applications.

The main changes in QP v4.1.00 with respect to earlier versions are as
follows:

- in qs.h added a new trace record QS_QEP_DISPATCH logged when an event
is dispatched to a state machine. This timestamped trace record marks
the beginning of an RTC step. The end of the RTC step is marked by the
existing timestamped trace records QS_QEP_INTERN_TRAN, QS_QEP_INIT_TRAN,
and QS_QEP_IGNORED, depending on how the event is handled. The new
QS_QEP_DISPATCH record facilitates measurement of the RTC step lengths.

- in qhsm_dis.c added generation of the QS_QEP_DISPATCH trace record.

- in the tools\qspy\ sub-directory added output of the new trace record
to the Q-SPY host application.

- in the tools\qspy\matlab\ sub-directory added processing of the new
trace record to the qspy.m script.

- in qpset.h changed the implementation of the Priority Set. In
particular, the QPSet64 now derives from QPSet8, which enables a common
way of testing for non-empty set (e.g., useful in assembly). Also, the
findMax() functions in QPSet8 and QPSet64 now can work with an empty
set, in which case they return 0.

- in qk_sched.c simplified the QK_schedule_() function to skip the
testing of the ready-set for non-empty condition. Such test is no longer
necessary. The test can still be performed outside the QK_schedule_()
function (e.g., in assembly) to avoid calling the scheduler if the ready
set is empty.

- in qk_ext.c simplified the QK_scheduleExt_() function in the same way
as QK_schedule_().

- modified make.bat files for building QP libraries in the ports\
directory to use the externally defined environment variables for the
location of the toolchains. The defaults are applied only when the
environment variable is not defined. This enables greater flexibility in
installing the development tools in different directories than those
chosen by Quantum Leaps.

- modified the ARM Cortex-M3 examples for the new IAR EWARM 5.40.

- modified slighlty the Calculator example to allow extensibility.

- in the ARM Cortex-M3 port file qk_port.s added explicit testing of the
QK_readySet_ set for empty condition. This test allows avoiding calling
the QK scheduler and two contex-switches if the ready-set is empty.

- in the game example moved setting up the QS filters from main.c to bsp.c.


QP/C Version 4.0.04 (09-Apr-2009)
---------------------------------
The maintenance release provides a fix for the compile-time assertions,
which did not work correctly for the GNU compiler family. Also, the ARM
Cortex-M3 examples have been recompiled with the newer IAR EWARM v5.30.

This maintenance release does not change the QP/C API in any way, so the
release has NO IMPACT on the QP/C applications.

The main changes in QP v4.0.04 with respect to earlier version are as
follows:

- in qassert.h file the #Q_ASSERT_COMPILE macro has been modified to render
a negative array dimension when the asserted condition is not TRUE.


QP/C Version 4.0.03 (27-Dec-2008)
---------------------------------
The main purpose of this release is to fix a bug in the QK preemptive
kernel, which occurs only when the advanced QK features are used.
Specifically, the QK priority-ceiling mutex could interfere with QK
thread-local storage (TLS) or QK extended context switch. When the QK
mutex is in use, the TLS or the extended context for this task could get
saved to an incorrect priority level.

The release 4.0.03 fixes the bug by strictly preserving the semantics of
QK_currPrio_ variable. The mutex locking now uses a different variable
QK_ceilingPrio_, which represents the ceiling-priority locked by the
mutex. The QK scheduler and extended scheduler now perform an additional
check to make sure that only tasks with priorities above the ceiling can
run. To avoid that additional overhead, the user can define the macro
QK_NO_MUTEX, which eliminates the QK mutex API and eliminates the
additional tests in the QK schedulers.

This maintenance release does not change the QP/C API in any way, so the
release has NO IMPACT on the QP/C applications.

The main changes in QP v4.0.03 with respect to earlier version are as
follows:

- in qk.h file made the QK mutex API only visible when the macro
QK_NO_MUTEX is *not* defined.

- in qk_pkg.h file, added the QK_ceilingPrio_ external declaration when
the macro QK_NO_MUTEX is not defined.

- in qk_mutex.c file, changed priority-ceiling mutex implementation to
use the QK_ceilingPrio_ instead of QK_currPrio_. Also, added compiler
error when the macro QK_NO_MUTEX is definedthis and this file is
included in the build.

- in qk_sched.c file added testing priority against the QK_ceilingPrio_,
when the macro QK_NO_MUTEX is not defined.

- in qk_ext.c file added testing priority against the QK_ceilingPrio_,
when the macro QK_NO_MUTEX is not defined.


QP/C Version 4.0.02 (15-Nov-2008)
---------------------------------
This maintenance release does not change the QP/C API in any way, so the
release has NO IMPACT on the QP/C applications.

The main changes in QP v4.0.02 with respect to earlier version are as
follows:

- in qep.h file, added comments to macros Q_TRAN() and Q_SUPER() to
suppress the PC-lint warining about using the comma-operator (MISRA rule
42).

- in qhsm_in.c file, fixed a bug in the QHsm_isIn() function.

- fixed a bug in tunnel.c file ("Fly 'n' Shoot" game). The constant
event HIT_WALL was not declared static.


QP/C Version 4.0.01 (09-Jun-2008)
---------------------------------
This maintenace release is made to allow using QS software tracing with
the GNU compiler for AVR (WinAVR). Specifically, the output of the
strings residing in ROM has been fixed.

This maintenance release does not change the QP/C API in any way, so the
release has NO IMPACT on the QP/C applications.

The main changes in QP v4.0.01 with respect to earlier version are as
follows:

- in qs_.c file, updated the function QS_str_ROM_().

- in qs_str.c file, updated the function QS_str_ROM().

- in qvanilla.c file, funciton QF_run(), declared the temporary
variables as static to save stack space, because QF_run() never returns
and is not reentrant.


QP/C Version 4.0.00 (07-Apr-2008)
=================================
This milestone release is made for the book "Practical UML Statecharts
in C/C++, Second Edition" [PSiCC2]
(http://www.quantum-leaps.com/psicc2/). The book describes in great
detail this new release. The older "QP Programmer's Manual" is now
phased out and is replaced with this hyper-linked "QP/C Reference
Manual" (http://www.state-machine.com/doxygen/qpc/tutorial.html), which
provides very detailed, easily searchable reference to the software. The
[PSiCC2] book provies in-depth discussion of the relevant concepts as
well as the design study of QP v4.0.

The main changes in QP v4.0 with respect to earlier versions are as follows:

- the coding techniques for hierarchical state machines (HSMs) and the
simpler finite state machines (FSMs) have changed. While the changes are
quite simple, the backward compatibility with QEP 3.x has been broken,
meaning that some manual changes to the state machines implemented with
earlier versions are necessary. Please refer to Chapter 4 in [PSiCC2]
for more information about coding state machines with QEP 4.x.

- The main change is the signature of a state-handler function, which
now returns simply a byte. This return type (typedef'ed as QState) is
the status of the event-handling that the state handler conveys to the
QEP event processor.
    - The macro Q_TRAN() must now always follow the return
      statement.
    - The new macro Q_SUPER() designates the
      superstate of the given state. Again, this macro must follow
      the return statement.
    - Then two new macros Q_HANDLED() and Q_IGNORED() have been added
      to return the status of event handled and event ignored,
      respectively.

- all callback functions are now consistently called XXX_onYYY()
    - Q_assert_handler() is now Q_onAssert()
    - QF_start() is now QF_onStartup()
    - QF_cleanup() is now QF_onCleanup()

- the new header file qevent.h has been broken off the qep.h header
file. qevent.h contains the QEvent class and other basic facilities used
in the whole QP. This new file allows easier replacement of the entire
QEP event processor with custom event processors, if you wish do so.

- the macro QEP_SIGNAL_SIZE is renamed to Q_SIGNAL_SIZE.

- the data type QSTATE is now deprecated. Please use QState.

- the "protected" in C don't no longer have the trailing underscore. For
example, QHsm_ctor_() is replaced with QHsm_ctor(), etc.

- the QF_FSM_ACTIVE macro is now deprecated. Instead, you have the
family of macros QF_ACTIVE_SUPER_, QF_ACTIVE_CTOR_, QF_ACTIVE_INIT_,
QF_ACTIVE_DISPATCH_, QF_ACTIVE_STATE_, which allow replacing the base
class for active objects in QF. By default, these macros are defined to
use the QHsm class from the QEP hierarchical event processor, but you
can replace the event processor, if you wish.

- the internal macro QACTIVE_OSOBJECT_WAIT_() is now
QACTIVE_EQUEUE_WAIT_().

- the internal macro QACTIVE_OSOBJECT_SIGNAL_() is now
QACTIVE_EQUEUE_SIGNAL_().

- the internal macro QACTIVE_OSOBJECT_ONIDLE_() is now
QACTIVE_EQUEUE_ONEMPTY_().

- the data mebers QActive.osObject and QActive.thread are now present
only if the macros QF_OS_OBJECT_TYPE and QF_THREAD_TYPE are defined.

- the QPSet class has been renamed to QPSet64.

- the QPSet_hasElements() has been renamed QPSet64_notEmpty()

- the QS software tracing is now better integrated with all QP
components. You no longer need to explicitly include qs_port.h,
because it is automatically included when you define the macro
Q_SPY. Also the file qs_dummy.h is included automatically when
the macro Q_SPY is <STRONG>not</STRONG> defined.

- the new header file qvanilla.h now replaces the file qsched.h.

- the file qa_ctor.c is now obsolete.

- the macros QF_SCHED_LOCK() and QF_SCHED_UNLOCK() are now obsolete.

- the native QF event queues (both the active object event queues and
the "raw" thread-safe queues) are slightly more efficient by counting
down the head and tail pointers rather than up. This leads to
wrap-around at zero, which is easier (faster) to test than any other
wrap-around point. Also the native QF event queues maintain the minimum
of the free events in the queue rather the maximum of used events.

- the data member of QEQueue.nTot class is removed.

- the QF_publish() function has been re-written so that QF no does
<STRONG>not need to lock the scheduler</STRONG>. The QF_publish()
function posts events to active objects with scheduler unlocked starting
from the highest-priority active objects. However, the event is
protected from inadvertent recycling by incrementing its reference
counter before the publish operation. After the event  is posted to all
subscribers, the garbage collector QF_gc() is called to decrement the
reference counter and recycle the event, if necessary.

- the qf_run.c file is obsolete. The QF_run() function for the
cooperative "vanilla" kernel is now implemented in the file qvanilla.c.

- the QF_tick() function has been rewritten to allow calling QF_tick()
from the task context as well as from the interrupt context. The nested
critical section around QF_tick() is no longer needed when it is called
from the task level. Among others, this re-design <STRONG>eliminates the
need for the recursive</STRONG> mutex in the POSIX QF port.

- the QMPool_init() function has been re-designed to optimally align the
memory buffer in a portable and platform-independent way. This should
bring some performance improvements on some CPUs (e.g., 80x86).

- the extended QK scheduler has been re-designed to save stack space.
The extended context (e.g., coprocessor registers) are no longer saved
on the precious stack, but rather in the active object.

- a bug has been fixed in handling of Thread-Local Storage (TLS) in the
QK scheduler and extended scheduler.

- the -q (quiet) flag has been added to the QSPY host application.

- the support for two new compilers for Windows has been added for the
QSPY host application. The application can now be build with the MinGW
GNU compiler for Windows as well as the Microsoft Visual C++ 2005.

- the QP port to Linux has been improved by eliminating the need for
recursive P-Thread mutex.

- the QP port to MicroC/OS-II has been upgraded to the latest version 2.86.

- all examples in the standard QP distribution have been cleaned up and
updated to the latest QP API changes.

- all examples that use QF now contain the QS software tracing support.



