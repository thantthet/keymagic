#----- set the project name --------------------------------------------------------------
Proj = TextService

#----- Include the PSDK's WIN32.MAK to pick up defines -----------------------------------
!include <win32.mak>

#----- OUTDIR is defined in WIN32.MAK This is the name of the destination directory ------
all: $(OUTDIR)\$(Proj).dll

LINK32_OBJS= \
	$(OUTDIR)\TextService.obj \
	$(OUTDIR)\ThreadMgrEventSink.obj \
	$(OUTDIR)\TextEditSink.obj \
	$(OUTDIR)\LanguageBar.obj \
	$(OUTDIR)\KeyEventSink.obj \
	$(OUTDIR)\Compartment.obj \
	$(OUTDIR)\KeyHandler.obj \
	$(OUTDIR)\StartComposition.obj \
	$(OUTDIR)\EndComposition.obj \
	$(OUTDIR)\Composition.obj \
	$(OUTDIR)\DisplayAttributeProvider.obj \
	$(OUTDIR)\DisplayAttributeInfo.obj \
	$(OUTDIR)\EnumDisplayAttributeInfo.obj \
	$(OUTDIR)\DisplayAttribute.obj \
	$(OUTDIR)\Dllmain.obj \
	$(OUTDIR)\Globals.obj \
	$(OUTDIR)\PreComp.obj \
	$(OUTDIR)\Register.obj \
	$(OUTDIR)\Server.obj
        
#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

#----- add the preprocessor definitions needed by this project ---------------------------

#----- set the libraries needed by this project ------------------------------------------
LINK_LIBS = $(dlllibs) $(guilibs) $(olelibs)

#--------------------- DLL ---------------------------------------------------------------
#PLEASE REFER TO WIN32.MAK for the different Linker options WIN32.MAK provides

# Build rule for DLL
$(OUTDIR)\$(Proj).dll: $(OUTDIR) $(LINK32_OBJS) $(OUTDIR)\$(Proj).res
    $(link) $(ldebug) $(dllllflags) \
    $(LINK32_OBJS) $(OUTDIR)\$(Proj).res $(LINK_LIBS) \
    -base:0x20000000   \
    -out:$(OUTDIR)\$(Proj).dll \
    -def:$(Proj).def

#--------------------- Compiling C/CPP Files ---------------------------------------------
#PLEASE REFER TO WIN32.MAK for the different Compiler options WIN32.MAK provides

FILE=TextService

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=ThreadMgrEventSink

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=TextEditSink

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=LanguageBar

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=KeyEventSink

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=Compartment

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=KeyHandler

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=StartComposition

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=EndComposition

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=Composition

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=DisplayAttributeProvider

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=DisplayAttributeInfo

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=EnumDisplayAttributeInfo

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=DisplayAttribute

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=Dllmain

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=Globals

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=Precomp

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=Register

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp

FILE=Server

$(OUTDIR)\$(FILE).obj : .\$(FILE).cpp $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" .\$(FILE).cpp


#--------------------- Compiling Resource Files ------------------------------------------
#PLEASE REFER TO WIN32.MAK for the different Resource Compiler options WIN32.MAK provides

# Build rule for resource file
FILE=TextService

$(OUTDIR)\$(FILE).res: .\$(FILE).rc $(OUTDIR)
    $(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\$(FILE).res .\$(FILE).rc

#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)
