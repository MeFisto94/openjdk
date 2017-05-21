# Fork of openJDK
This Fork aims to provide support for building the JVM (JRE) for use in UWP Applications.
This means you will lateron just use `jni.h`, `jvm.lib`, `jvm.dll` and `java.dll` and can
run a JVM from code, so you can deploy regular UWP Applications launching Java Code.  

Currently the build stops during the JDK compilation (`awt.dll`) however those files aren't
needed in the above use-case, they are needed when you want to call awt draw functions
directly from native code. If you can solve the build problem, PRs are welcomed, though.  

When actually using the `jvm.dll` with the boilerplate code below (don't forget to link
`jvm.lib`, add `jni.h`'s path to the include dir and add `jvm.dll` and `java.dll` to your
App (Project) and set `Content` to `true` in these files properties), it fails to initialize.  

This is because the classloader cannot find `java/lang/Object`. I tried to specify both
another classpath (`-Djava.class.path=...`) as well as `-Xbootclasspath` (which sets the
system class path (I checked that), so it should be used definately before the vm is fully
initialized). I also tried just placing the `rt.jar` into the path where the original
bootclasspath was (namely in the dir where the exe is located). It just didn't work,
I guess the internal dictionaries didn't got populated, but I have no clue where they
are populated in code.  

If you take the `jvm.diz` file and unzip it, you get the pdb. Also add it to your project
as described above and you can fully debug the jdk's sourcecode.  

## Building
The Buildprocess is the most cumbersome part, however I fixed plenty things already,
so for you it is a matter of setting up your environment.  

You need:
* Probably Windows 10 (at least that's what I have)
* MS Visual Studio 2017 (Community Version is sufficient and for free). Probably 2k15 also works
* Installed Windows 10 SDK (Can be done through the Visual Studio)
* MSYS 1 (MSYS 2 doesn't work, maybe MinGW64 does, which you can invoke through msys2)
* Built LibFreeType (see below)

When you are ready to roll you have to build libfreetype as it's explained
[here](https://web.archive.org/web/20150403031533/https://weblogs.java.net/blog/simonis/archive/2011/10/28/yaojowbi-yet-another-openjdk-windows-build-instruction#FreeType)
Since that site is already archived, I'll extract it's contents below (with modifications)

> Download FreeType from http://www.freetype.org/download.html, extract it somewhere (ensure it's a path WITHOUT spaces!)
and doubleclick on builds\win32\vc2010\freetype.vcxproj to open the FreeType project
and let VS2017 upgrade it. From the projects properties do the following:  
>   Configuration Manager -> Active Solution Manager -> Type or select the new Platform -> x64  
>   Configuration -> Release Multithreaded  
>   Platform -> x64  
>   Target Name -> rename to "freetype"  
>   Platform Toolset -> Windows10SDK  
>	CompileSwitch /MT to /MD (Found under C++ somewhere, Linking Type for the Runtime)    

> Then choose "Release Multithreaded"/"x64" in the menu bar and Build the project by choosing "Build" from the project menu.  
> This will create the 64-bit freetype.lib in the corresponding output directory.
> Now change the "Configuration Type" to "dll" in the project properties and build again.  
> Now the 64-bit freetype.dll will be build in the output directory.    

When you have that, you can start msys. You cannot start the regular msys however, you have
to open the 64-bit Version of the Visual Studio Command Prompt First. Open your Start Menu,
Go to the Folder `Visual Studio 2017` (Searching did not work for me?) and click on
`x64 Native Tools Command Prompt for VS 2017`. You should see a command window popping up.
Now you have to launch msys from that prompt. You could do this by typing in the path to
the `msys.bat` or you could also just drag and drop it.  

Now move to the location of this repo using `cd /c/Users/etc/you/understand/how/this/works`.
When you are here, type in `common/autoconf/autogen.sh`, this will re-build the configure tool
since we regularly change compileflags and they are picked up here.  

Now another hard part: Since autoconf/configure cannot deal with toolpaths consisting of spaces,
and since VS 2017 is installed in a folder called `Visual Studio`, we have to bypass this.
If your VS Path is different, you have to adjust this accordingly:
`ln -s /e/Programme/Microsoft\ Visual\ Studio/2017/Community/VC/Tools/MSVC/14.10.25017/bin/Hostx64/x64/ /tmp/tools`
This will link the path /tmp/tools to the real directory.

Now we will invoke configure. If you experience any issues there, things get funky.
Call `./configure TOOLS_DIR="/tmp/tools" --with-freetype-include="../../freetype-2.8/include/" --with-freetype-lib="../../freetype-2.8/objs/vc2010/x64`
As you can see, you have to adjust the last two switches to the directory where you placed freetype.

Now since configure ran, we are ready to build!!! First `cd build/windows*` to enter the build directory.
It depends on which issue you want to tackle: You can now enter `make hotspot`, which only compiles the jvm,
which is what we actually need. This has a much higher priority actually.  

When you type `make jdk`, the build will crash at a linker problem, since for some reason, many objs of libawt still
use the static version of the c runtime `libc(p)mt.lib`. You can see this when you add `-NODEFAULTLIB libcpmt.lib` in
`jdk/make/lib/Awt2dLibraries.gmk` in Line 479 (LDFLAGS). 


## The example program to launch the jvm
```cpp
#include <jni.h>       /* where everything is defined */
...
JavaVM *jvm;       /* denotes a Java VM */
JNIEnv *env;       /* pointer to native method interface */
JavaVMInitArgs vm_args; /* JDK/JRE 8 VM initialization arguments */
JavaVMOption* options = new JavaVMOption[1];
options[0].optionString = "-Djava.class.path=PATH_TO_rt.jar";
vm_args.version = JNI_VERSION_1_8;
vm_args.nOptions = 1;
vm_args.options = options;
vm_args.ignoreUnrecognized = false;
/* load and initialize a Java VM, return a JNI interface
 * pointer in env */
JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
delete options;
/* invoke the Main.test method using the JNI */
jclass cls = env->FindClass("Main");
jmethodID mid = env->GetStaticMethodID(cls, "test", "(I)V");
env->CallStaticVoidMethod(cls, mid, 100);
/* We are done. */
jvm->DestroyJavaVM();
```

As you can see there are still two more things to do: Create a UWP Project in Visual Studio and add this
code inside of that Apps Constructor (it's the easiest way).
You will also have to adjust the `-Djava.class.path` to point to YOUR already installed jdk/jre, since 
I think our build hasn't come thus far yet. Anyhow this switch is the keyproblem: For some reason the
classes aren't searched at that path, I don't know...

Now: Add the `jvm.dll`, `jvm.pdb` and `java.dll` to your project and set them as content (as described above)
and start the program in debugging mode, stepping through the code starting at `JNI_CreateJavaVM` using
the debugger. What you want is to place a debugger in universe.cpp:302 (SystemDictionary::initialize(CHECK);)
so you can skip until that point during execution. Now you want to find out why already the first loaded class
`java/lang/Object` fails.

## original Readme
README:
  This file should be located at the top of the OpenJDK Mercurial root
  repository. A full OpenJDK repository set (forest) should also include
  the following 6 nested repositories:
    "jdk", "hotspot", "langtools", "corba", "jaxws"  and "jaxp".

  The root repository can be obtained with something like:
    hg clone http://hg.openjdk.java.net/jdk8/jdk8 openjdk8
  
  You can run the get_source.sh script located in the root repository to get
  the other needed repositories:
    cd openjdk8 && sh ./get_source.sh

  People unfamiliar with Mercurial should read the first few chapters of
  the Mercurial book: http://hgbook.red-bean.com/read/

  See http://openjdk.java.net/ for more information about OpenJDK.

Simple Build Instructions:
  
  0. Get the necessary system software/packages installed on your system, see
     http://hg.openjdk.java.net/jdk8/jdk8/raw-file/tip/README-builds.html

  1. If you don't have a jdk7u7 or newer jdk, download and install it from
     http://java.sun.com/javase/downloads/index.jsp
     Add the /bin directory of this installation to your PATH environment
     variable.

  2. Configure the build:
       bash ./configure
  
  3. Build the OpenJDK:
       make all
     The resulting JDK image should be found in build/*/images/j2sdk-image

where make is GNU make 3.81 or newer, /usr/bin/make on Linux usually
is 3.81 or newer. Note that on Solaris, GNU make is called "gmake".

Complete details are available in the file:
     http://hg.openjdk.java.net/jdk8/jdk8/raw-file/tip/README-builds.html
