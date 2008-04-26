import os
import sys
import string

class BuildError(Exception):
    def __init__(self, value):
        self.value = value
        
    def __repr__(self):
        return repr(self.value)

def runInDir(command, dir=None):
    if dir:
        olddir = os.getcwd()
        os.chdir(dir)
        
    result = os.system(command)
    
    if dir:
        os.chdir(olddir)
        
    return result

class Builder:
    """
    Base class exposing the Builder interface.
    """
    
    def __init__(self, formatName="", commandName="", programDir=None):
        """
        formatName = human readable name for project format (should correspond with Bakefile names)
        commandName = name of command line program used to invoke builder
        programDir = directory program is located in, if not on the path
        """
        
        self.dir = dir
        self.name = commandName
        self.formatName = formatName
        self.programDir = programDir
        self.doSetup()
        
    def doSetup(self):
        """
        Do anything special needed to configure the environment to build with this builder.
        """
        
        pass

    def isAvailable(self):
        """
        Run sanity checks before attempting to build with this format
        """

        # Make sure the builder program exists
        programPath = self.getProgramPath()
        if os.path.exists(programPath):
            return True
        else:
            # check the PATH for the program
            # TODO: How do we check if we're in Cygwin?
            if sys.platform.startswith("win"):
                dirs = os.environ["PATH"].split(":")
                
                for dir in dirs:
                    if os.path.isfile(os.path.join(dir, self.name)):
                        return True  
            
            else:
                result = os.system("which %s" % self.name)
        
                if result == 0:
                    return True
            
        return False
        
    def getProgramPath(self):
        if self.programDir:
            return os.path.join(self.programDir, self.name)
        else:
            return self.name
        
    def clean(self, dir=None, projectFile=None):
        """
        dir = the directory containing the project file
        projectFile = Some formats need to explicitly specify the project file's name
        """
        if self.isAvailable():
            result = runInDir("%s clean" % self.getProgramPath(), dir)
            return result
            
        return False
        
    def configure(self, options=None):
        # if we don't have configure, just report success
        return True
            
    def build(self, dir=None, projectFile=None, targets=None, options=None):
        if self.isAvailable():
            optionsStr = string.join(targets, " ") if targets else ""
            optionsStr += string.join(options, " ") if options else ""
            result = runInDir("%s %s" % (self.getProgramPath(), optionsStr), dir)

            return result
        
        return False

    def install(self, dir=None):
        if self.isAvailable():
            result = runInDir("make install", dir)
            return result
            
        return False

# Concrete subclasses of abstract Builder interface

class GNUMakeBuilder(Builder):
    def __init__(self, commandName="make", formatName="GNUMake"):
        Builder.__init__(self, commandName=commandName, formatName=formatName)
        

class XCodeBuilder(Builder):
    def __init__(self, commandName="xcodebuild", formatName="XCode"):
        Builder.__init__(self, commandName=commandName, formatName=formatName)

        
class AutoconfBuilder(GNUMakeBuilder):
    def __init__(self, formatName="autoconf"):
        GNUMakeBuilder.__init__(self, formatName=formatName)
        
    def configure(self, options=None):
        #olddir = os.getcwd()
        #os.chdir(dir)
        
        configdir = os.getcwd()
        configure_cmd = ""
        while os.path.exists(configdir):
            config_cmd = os.path.join(configdir, "configure")
            if not os.path.exists(config_cmd):
                configdir = os.path.abspath(os.path.join(configdir, ".."))
            else:
                configure_cmd = config_cmd
                break
                
        if not configure_cmd:
            raise BuildError, "Could not find configure script at %s. Have you run autoconf?" % dir
        
        optionsStr = string.join(options, " ") if options else ""
        result = os.system("%s %s" % (configure_cmd, optionsStr))
        #os.chdir(olddir)
        return result
        

class MSVCBuilder(Builder):
    def __init__(self):
        Builder.__init__(self, commandName="nmake", formatName="msvc")
        

builders = [GNUMakeBuilder, XCodeBuilder, AutoconfBuilder, MSVCBuilder]


def getAvailableBuilders():
    availableBuilders = {}
    for symbol in builders:
        thisBuilder = symbol()
        if thisBuilder.isAvailable():
            availableBuilders[thisBuilder.formatName] = symbol
                
    return availableBuilders