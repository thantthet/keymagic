PROGRAM = KeyMagic
DISTDIR = ./dist
DEPSDIR = ./deps
BINARIES = /tmp/KeyMagic.dst
DMGFILE = $(PROGRAM).dmg
PRODUCT = $(DISTDIR)/$(PROGRAM).pkg
COMPONENT = $(DEPSDIR)/$(PROGRAM)Component.pkg
COMPONENT_PFILE = $(PROGRAM).plist
DISTRIBUTION_FILE = distribution.dist
REQUIREMENTS = requirements.plist

.PHONY : all
all : $(DISTDIR) $(DEPSDIR) $(PRODUCT) $(DMGFILE)

$(DISTDIR) :
  mkdir $(DISTDIR)

$(DEPSDIR) :
  mkdir $(DEPSDIR)

$(PRODUCT) : $(BINARIES) $(REQUIREMENTS) \
             $(DEPENDENCY) $(COMPONENT_PFILE) \
             $(COMPONENT) $(DISTRIBUTION_FILE)
  productbuild --distribution $(DISTRIBUTION_FILE) \
    --resources . \
    --package-path $(DEPSDIR) \
    --package-path $(DEPENDENCYDIR) \
    $(PRODUCT)

$(BINARIES) :
  xcodebuild install

$(COMPONENT_PFILE) :
  @echo "Error: Missing component pfile."
  @echo "Create a component pfile with make compfiles."
  @exit 1

$(COMPONENT) : $(BINARIES) $(COMPONENT_PFILE)
  pkgbuild --root $(BINARIES) \
  --component-plist $(COMPONENT_PFILE) \
  $(COMPONENT)

$(DISTRIBUTION_FILE) :
  @echo "Error: Missing distribution file."
  @echo "Create a distribution file with make distfiles."
  @exit 1