#!/usr/bin/env python

#
# Generates Makefile that is used to regenerate native makefiles from
# bakefiles.
#
# $Id$
#

import string, os.path

file = open('Makefile', 'wt')
file.write("""
# Generated by regenMakefile.py

BAKEFILE = bakefile
BAKEARGS = -v


""")

lines = []
all = {}
all['autoconf'] = ['../../configure']

def addMakefile(bake, makedirs, deps=[], args={}):
    """Adds rules to regenerate native makefile in directory 'makedir' from
       bakefiles 'bake'. 'deps' contains additional dependencies (bakefiles
       other than 'bake'."""
    print 'adding %s...' % bake
    lines.append('')
    lines.append('# from %s' % bake)

    def add(bake, makedirs, make, dep, format, args={}):
        a = ''
        if 'all' in args: a += ' %s' % args['all']
        if format in args: a += ' %s' % args[format]
        if format in makedirs:
            makedir = makedirs[format]
        else:
            makedir = makedirs['all']
        tfile = '%s/%s' % (makedir, make)
        lines.append('%s: %s' % (tfile, dep))
        lines.append('\t$(BAKEFILE) $(BAKEARGS) -f%s -o%s%s %s' % \
                     (format, tfile, a, bake))
        lines.append('\ttouch %s' % tfile)
        if format not in all: all[format] = []
        all[format].append(tfile)

    dep = string.join(deps + [bake], ' ')

    add(bake, makedirs, 'Makefile.in', dep, 'autoconf', args)



# -----------------------------------------------
#    Add the makefiles:
# -----------------------------------------------

# main makefile:
addMakefile('wx.bkl', {'all':'..','autoconf':'../..'},
            ['common.bkl', 'config.bkl', 'files.bkl', 'monolithic.bkl',
             'wxwin.py'])

# samples main makefile:
addMakefile('../../samples/samples.bkl', {'all':'../../samples'})


def onSample(arg, dirname, names):
    bakes = [x for x in names if x.endswith('.bkl')]
    if len(bakes) == 0: return
    depth = dirname.count('/') - 2
    if depth <= 0: return
    prefix = ''.join(['../' for i in range(0,depth)])

    args = {
        'all':'-DWXTOPDIR=/%s..' % prefix,
        'autoconf':'-DAUTOCONF_MACROS_FILE=../../autoconf_inc.m4',
    }
    
    for bake in bakes:
        addMakefile('%s/%s' % (dirname, bake), {'all':dirname},
                    deps=['common.bkl','common_samples.bkl','config.bkl'],
                    args=args)

os.path.walk('../../samples', onSample, None)


cleanCmds = ''
for f in all:
    for i in all[f]:
        cleanCmds += '\trm -f %s\n' % i

for f in all:
    var = '%s_ALL' % f.upper()
    file.write('%s = %s\n' % (var,' '.join(all[f])))

file.write('all:')
for f in all:
    file.write(' %s' % f)
file.write('\n\n')
for f in all:
    file.write('%s: $(%s_ALL)\n' % (f, f.upper()))

file.write("""
clean:
\trm -f ../../autoconf_inc.m4
%s

../../autoconf_inc.m4: ../../Makefile.in
../../configure: ../../autoconf_inc.m4
\t(cd ../.. ; aclocal && autoconf)

Makefile: regenMakefile.py
\t./regenMakefile.py
\t@echo
\t@echo -------------------------------------------
\t@echo Please rerun make, Makefile was regenerated
\t@echo -------------------------------------------
\t@echo
\t@exit 1
""" % cleanCmds)
for l in lines:
    file.write('%s\n' % l)
file.close()
