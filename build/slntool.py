#!/usr/bin/env python
#
# @brief Visual Studio solution generator
# @date $Date: 2008-02-29 05:05:45 $
# @author Norkai Ando <n-ando@aist.go.jp>
#
# Copyright (C) 2008
#     Noriaki Ando
#     Task-intelligence Research Group,
#     Intelligent Systems Research Institute,
#     National Institute of
#         Advanced Industrial Science and Technology (AIST), Japan
#     All rights reserved.
#
# $Id: slntool.py,v 1.1.2.1 2008-02-29 05:05:45 n-ando Exp $
#

# $Log: not supported by cvs2svn $
#

import sys
import re
import yaml
import yat

#------------------------------------------------------------
# Generic vcproj template
#------------------------------------------------------------
sln_template = """Microsoft Visual Studio Solution File, Format Version 9.00
# Visual Studio 2005
[for proj in Projects]
Project("{[SolutionGUID]}") = "[proj.Name]", "[proj.FileName]", "{[proj.GUID]}"
	ProjectSection(ProjectDependencies) = postProject
[if-any proj.Depend]
[for dep in proj.Depend]
		{[dep]} = {[dep]}
[endfor]
[endif]
	EndProjectSection
EndProject
[endfor]
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
[for conf in Configurations]
		[conf] = [conf]

[endfor]
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
[for proj in Projects]
[for conf in Configurations]
		{[proj.GUID]}.[conf].ActiveCfg = [conf]

		{[proj.GUID]}.[conf].Build.0   = [conf]

[endfor]
[endfor]
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
EndGlobal
"""


sln_yaml = """
SolutionGUID: 8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942
Configurations:
  - Release|Win32
  - Debug|Win32
"""


def usage():
    print """
Usage:
  slntool.py --dep dep_file [--outfile outfile] vcproj_files...

Options:
    --dep: dependency file
    --out or --output: output file name

Example:
 * make solution from all of found vcprojs
  slntool.py --dep dep_file --outfile MyApp.sln \
    `find ./ --name '*.vcproj'`
 * output solution to stdout
  slntool.py --dep dep_file *.vcproj

Depfile examples: The rule is similar to Makefile's dependency rule
  All of entries should be Projectname.
  [target Projectname]: [dependent projectsname...]

In the following App.dep file, "App" dpends on "Lib1" and "Lib2".
"Lib2" depends on "Lib1", "Lib1" has no dependency.
For this solution, "App", "Lib1" and "Lib2"'s project files are necessary.

-- App.dep --
App: Lib1 Lib2
Lib2: Lib1
-------------

"""


def get_projinfo(fname):
    name = None
    guid = None
    re_guid = re.compile('^.*?ProjectGUID=\"{(.*)}\"')
    re_name = re.compile('^.*?Name=\"(.*)\"')
    fd = open(fname, "r")
    pj = fd.readlines()
    for t in pj:
        n = re_name.match(t)
        g = re_guid.match(t)

        if name == None and n:
            name = n.group(1)
        if guid == None and g:
            guid = g.group(1)

        if name and guid:
            break
    fd.close()
    return {"Name": name, "GUID": guid, "FileName": fname}

def get_dependencies(fname):
    if fname == None: return {}
    depdic = {}
    fd = open(fname, "r")
    for l in fd.readlines():
        (key, val) = l.split(":")
        vals = val.split()
        depdic[key] = vals
    return depdic

def parse_args(argv):
    argc = len(argv)
    depfile = None
    outfile = None
    flist = []
    i = 0
    while i < argc:
        opt = argv[i]
        if opt == "--dep":
            i += 1
            if i < argc: depfile = argv[i]
            else: raise InvalidOption(opt + " needs value")
        elif opt == "--output" or opt == "--out":
            i += 1
            if i < argc: outfile = argv[i]
            else: raise InvalidOption(opt + " needs value")
        else:
            while i < argc and argv[i][:2] != "--":
                flist.append(argv[i])
                i += 1
        i += 1
    return (depfile, outfile, flist)

def get_slnyaml(depfile, projfiles):
    depdict = get_dependencies(depfile)
    projs = []
    projlist = """Projects:
"""
    for f in projfiles:
        pj = get_projinfo(f)
        if depdict.has_key(pj["Name"]):
            pj["Depend"] = depdict[pj["Name"]]
        projs.append(pj)
    for pj in projs:
        list = """  - Name: %s
    FileName: %s
    GUID: &%s %s
    Depend:
""" % (pj["Name"], pj["FileName"], pj["Name"], pj["GUID"])
        if pj.has_key("Depend"):
            for dep in pj["Depend"]:
                dep = """      - *%s
""" % (dep)
                list += dep
        projlist += list
    yaml_text = sln_yaml + projlist
    return yaml_text
        
def gen_solution(yaml_text):
    dict = yaml.load(yaml_text)
    t = yat.Template(sln_template)
    return t.generate(dict)


class SlnToolException:
    pass

class InvalidOption(SlnToolException):
    def __init__(self, msg):
        self.msg = "Error: InvalidOption:\n    "
        self.msg += msg

#------------------------------------------------------------
# main function
#------------------------------------------------------------
def main(argv):
    if len(argv) == 0:
        usage()
        sys.exit(-1)
    try:
        res = parse_args(argv)
    except SlnToolException, e:
        print "\n" + e.msg + "\n"
        usage()
        sys.exit(-1)

    depfile = res[0]
    outfile = res[1]
    flist   = res[2]

    sln_text = gen_solution(get_slnyaml(depfile, flist))

    if outfile == None:
        fd = sys.stdout
    else:
        fd = open(outfile, "w")
    fd.write(sln_text)

#------------------------------------------------------------
# tests
#------------------------------------------------------------
def test_getprojinfo():
    for f in sys.argv[1:]:
        print get_projinfo(f)

def test_getdep():
    print get_dependencies(sys.argv[1])

def test_getslnyaml():
    print gen_solution(get_slnyaml("dep.yaml", sys.argv[1:]))

#------------------------------------------------------------
# entry point
#------------------------------------------------------------
if __name__ == "__main__":
    main(sys.argv[1:])
    
