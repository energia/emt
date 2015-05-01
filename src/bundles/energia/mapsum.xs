/*
 *  ======== mapsum.xs ========
 */

var usage = "usage: xs -c mapsum.xs [-t <toolchain>] [-v] mapfile";

var symbolNamesGnu = {
    __UNUSED_SRAM_start__: "SRAM_UNUSED_start",
    __UNUSED_SRAM_end__: "SRAM_UNUSED_end",
    __SRAM_LENGTH__: "SRAM_length",

    __UNUSED_FLASH_start__: "FLASH_UNUSED_start",
    __UNUSED_FLASH_end__: "FLASH_UNUSED_end",
    __FLASH_LENGTH__: "FLASH_length"
};

var symbolTable = {};
var objectTable = {};
var toolChain = null;

function main(arguments)
{
    var verbose = 0;

    for (;;) {
        var arg = arguments[0];
        if (arg == null || arg[0] != '-') {
            break;
        }

        switch (arg[1]) {
            case 'v': {
                verbose++;
                break;
            }
            case 't': {
                if (arguments[1] == null) {
                    print(usage);
                    return;
                }
                toolChain = arguments[1];
                arguments.shift();
                break;
            }
            default: {
                print(usage);
                return;
            }
        }
        arguments.shift();
    }
    if (arguments[0] == null) {
	print(usage);
	return;
    }

    var carray = parse(arguments[0], toolChain);

    print(arguments[0] + " summary:");
    display(carray, arguments[0], verbose);

    //findRef(carray, "_aeabi_(ddiv|dmul|dadd|dsub|dcmp)");
}

/*
 *  ======== getTargetSuffix ========
 */
function getTargetSuffix(carray)
{
    for (var i = 0; i < carray.length; i++) {
        var name = carray[i].name.split(/\s/)[0];
	var k = name.lastIndexOf('.') + 1;
	if (k != 0) {
	    var suffix = name.substring(k);
	    if (suffix[0] == 'o' || suffix[0] == 'a') {
		if (suffix != "obj" && suffix != "a") {
		    return (suffix.substring(1));
		}
	    }
	}
    }
    return (null);
}

/*
 *  ======== getToolChain ========
 */
function getToolChain(fileName)
{
    var result = null;
    
    var file = new java.io.BufferedReader(new java.io.FileReader(fileName));
    var line;

    for (var i = 0; i < 10 && (line = file.readLine()) != null; i++) {
	line = String(line);
	if (line.match(/^Archive member included/)) {
            result = "gnu";
	    break;
	}
	else if (line.match(/^>> Linked /)) {
	    result = "ti";
            break;
	}
    }
    file.close();
    
    return (result);
}

/*
 *  ======== parse ========
 */
function parse(fileName, tools)
{
    var result = null;

    /* if toolchain is not specified, try to figure it out from the map file */
    if (tools == null) {
	toolChain = getToolChain(fileName);
    }

    switch (toolChain) {
        case "gnu": {
            result = parseGnu(fileName);
            break;
        }
        case "ti": {
            result = parseTI(fileName);
            break;
        }
        default: {
            throw new Error("unknown toolchain '" 
			    + toolChain + "'; use 'ti' or 'gnu'.");
            break;
        }
    }
    return (result);
}

/*
 *  ======== parseGnu ========
 *  Return sorted array of "containers": objects with a total size
 *  and a hash of contributing sections and their size.
 * 
 *  Each element of the array is of the form:
 *   {
 *     name : <name of the "container" (library, object, ...)>, 
 *     total: <total size of the container>,
 *     sections: {<name of a contained "section">: <size of this contained section>}
 *   } 
 */
function parseGnu(fileName) 
{
    var file = new java.io.BufferedReader(new java.io.FileReader(fileName));
    var line;
    var result = {};

    var header = true;
    var continuation = false;
    var fill = 0;
    while ((line = file.readLine()) != null) {
	line = String(line);

        if (line.indexOf("Linker script and memory map") == 0) {
            header = false;
        }

        if (header) {
	    objectDepsGnu(line, objectTable);
	    continue;
	}

	/* look for symbol values for the symbols defined in symbolNamesGnu */
	if (symbolValueGnu(line, symbolNamesGnu, symbolTable)) {
	    continue;
	}

	/* now look for section allocation lines */
        var section; /* output section name */
        var size;    /* size of contained "section" */
        var key;     /* input container name + optional object name */
        var start;   /* start address */

        /* look for lines of the form:
         *    "^ <section_name> 0x<addr> 0x<size> <container>(<object>)",
         *    "^ <section_name>", or
         *    "^     0x<addr> 0x<size> <container>(<object>)" immediately followed by the form above
         */
        var tokens = line.match(/^ ([\.a-zA-Z0-9_+:\*]+)\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s+([\.a-zA-Z0-9_+\/\\\-]+)(\(.+\))?/);
        if (tokens == null) {

	    /* check for fill sections (alignment padding) */
	    tokens = line.match(/^\s+\*fill\*\s+0x[0-9a-f]+\s+(0x[\0-9a-f]+)/);
	    if (tokens != null && tokens[1] != null) {
		fill = tokens[1] - 0;
		continue;
	    }

	    /* are we expecting a continuation line? */
            if (continuation == true) {
                continuation = false;
                tokens = line.match(/^ \s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s+([\.a-zA-Z0-9_+\/\\\-]+)(\(.+\))?/);
                if (tokens == null) {
                    print("warning: expected a continuation of section '" + section + "', skipping line: " + line);
                    continue;
                }
                section += (tokens[4] ? tokens[4] : "");
                start = tokens[1] - 0;
                size = tokens[2] - 0;
                key = tokens[3];
            }
            else {
                tokens = line.match(/^ ([\.a-zA-Z0-9_+:\*]+)$/);
                if (tokens != null && tokens[1] != "CREATE_OBJECT_SYMBOLS") {
                    continuation = true;
                    section = tokens[1];
                }
                else {
                    section = null;
                }
                continue;
            }
        }
        else {
            section = tokens[1] + (tokens[5] ? tokens[5] : "");
            start = tokens[2] - 0;
            size = tokens[3] - 0;
            key = tokens[4];
            continuation = false;
        }

        if (size == 0 || section.match(/^(\.(debug|comment|stab|ARM\.attr|TI\.section\.flag|TI\.symbol))|xdc\.|__TI_DW\.debug/) != null) {
            continue;
        }

        /* treat COMMON sections as a single container */
        if (section == "COMMON") {
            section = key;
            key = "COMMON";
        }
            
        var index = Math.max(key.lastIndexOf('/'), key.lastIndexOf('\\'));
	var prefix = key.substring(0, index);
	if (java.io.File(prefix).isAbsolute()) {
	    prefix = String(java.io.File(prefix).getCanonicalPath());
	}
        key = key.substring(index + 1) 
	    + " (" + prefix.replace(/\\/g, '/') + ")";

        /* accumulate sizes for each "container" */
        if (result[key] == null) {
            result[key] = {name: key, total: 0, sections: {}, fill: 0};
        }
        result[key].total += size + fill;
        result[key].fill += fill;

        /* accumulate sizes of each contained section */
        var ssize = result[key].sections[section];
        result[key].sections[section] = size + (ssize != null ? ssize : 0);
	if (fill) {
	    /* add a separate fill section so we can see who needs alignment */
	    result[key].sections[section + "(*fill*)"] = fill;
	    fill = 0;
	}
    }

    /* close file stream */
    file.close();

    /* create array of containers and sort by size */
    var carray = [];
    for (var k in result) {
        carray.push(result[k]);
    }
    carray.sort(function (a, b) {return (b.total - a.total);});

    return (carray);
}

/*
 *  ======== parseTI ========
 *  Return sorted array of "containers": objects with a total size
 *  and a hash of contributing sections and their size.
 * 
 *  Each element of the array is of the form:
 *   {
 *     name : <name of the "container" (library, object, ...)>, 
 *     total: <total size of the container>,
 *     sections: {<name of a contained "section">: <size of this contained section>}
 *   } 
 */
function parseTI(fileName) 
{
    var file = new java.io.BufferedReader(new java.io.FileReader(fileName));
    var line;
    var result = {};
    while ((line = file.readLine()) != null) {
	line = String(line);

        /* look for lines of the form "  <hex_addr> <hex_size> <anything>" */
        var tokens = line.match(/        ([0-9a-f]+)    ([0-9a-f]+)     (.*)/);
        if (tokens != null) {
            var start = ("0x" + tokens[1]) - 0;
            var size = ("0x" + tokens[2]) - 0;
            var rest = tokens[3];

            /* parse "<anything>" for "<lib> : <obj> (<section>)" */
            tokens = rest.match(/(([\w\.\-]*) : )?([\w\.\-]+) \((.+)\)(\s\[.*\])?$/);
	    var lib;
	    var obj;
	    var section;
            if (tokens != null) {
                if (tokens[1] != null) { /* if there is a " : " qualifier */
                    if (tokens[2] != null && tokens[2].length > 0) {
			/* and if there is a word preceeding the : */
                        lib = tokens[2]; /* set the current library */
                    }
                }
                else {                   /* else there's no lib for this line*/
                    lib = null;
                }
                obj = tokens[3];
                section = tokens[4];
                //print((lib ? ("lib = " + lib + ", ") : "") + "obj = " + obj + ", section = " + section + ", size = " + size);
            }
            else if (rest.match(/\(\.common.+$/)) {
                lib = null;
                obj = null;
                section = rest.substring(1, rest.length - 1);
                //print("section = " + section + ", size = " + size);
            }
            else if (rest.match(/^--HOLE--/)) {
                obj = null;
                section = "--HOLE--";
                //print("section = " + section + ", size = " + size);
            }
            else {
                lib = null;
                obj = null;
                //print("huh?: " + rest);
                continue;
            }

            if (section == "xdc.meta" || section == "xdc.noload") {
                continue;
            }

            /* accumulate sizes for each "container" */
            var key = lib != null ? lib : (obj != null ? obj : ".common+holes");
            if (result[key] == null) {
                result[key] = {name: key, total: 0, sections: {}};
            }
            result[key].total += size;

            /* accumulate sizes of each contained section */
            var ssize = result[key].sections[section];
            result[key].sections[section] = size + (ssize != null ? ssize : 0);
        }
    }

    /* close file stream */
    file.close();

    /* create array of containers and sort by size */
    var carray = [];
    for (var k in result) {
        carray.push(result[k]);
    }
    carray.sort(function (a, b) {return (b.total - a.total);});

    return (carray);
}

/*
 *  ======== symbolValueGnu ========
 *  If line is a symbol definition and it's a symbol of interest, enter it's
 *  value in symbolTable.
 *
 *  We use two hash tables to enable TI and GNU tool chains to use different
 *  names for the same "value"; this enables the display method to be
 *  independent of the toolchain.
 */
function symbolValueGnu(line, symbolNames, symbolTable)
{
    var tokens = line.match(/^\s+(0x[a-fA-F0-9]+)\s+([a-zA-Z_]+[a-zA-Z0-9_+]*)/);
    if (tokens != null && tokens[1] != null && tokens[2] != null) {
	if (symbolNames[tokens[2]] != null) {
	    symbolTable[symbolNames[tokens[2]]] = Number(tokens[1]);
	    return (true);
	}
    }
    return (false);
}

/*
 *  ======== objectDepsGnu ========
 */
var cam = null;
function objectDepsGnu(line, objectTable)
{
    if (line.indexOf("Archive member ") == 0 || line == "") {
	return;
    }

    var amp = /^\s*([a-zA-Z-0-9_+\\\/:]+)\(([a-zA-Z-0-9_+\\\/\.:]+)\)(\s+\(([a-zA-Z-0-9_+\.]+)\))$/;
    var tokens = line.match(amp);
    if (tokens) {
	if (line[0] == ' ') {
	    objectTable.push({member: cam, referer: am, symbol: tokens[4]});
	}
	else {
	    cam = {name: line, archive: tokens[1], object: tokens[2]};
	}
    }
}

/*
 *  ======== findRef ========
 */
function findRef(carray, name)
{
    print("\n  scanning for " + name + " ...");
    if (toolChain != "gnu") {
	print("    can't parse non-gcc map files (yet).");
	return;
    }

    /* create a hash table of all objects linked into the program */
    var objects = {};
    for (var i = 0; i < carray.length; i++) {
        var c = carray[i];

	var isLib = false;
	var objName;
        for (var k in c.sections) {
	    var names = k.match(/^([a-zA-Z-0-9_+\.:]+)\(([a-zA-Z-0-9_+\.\\\/]*)\)/);
	    if (names && names[2] != null) {
		isLib = true;
		objName = names[2];
		if (objects[objName] == null) {
		    objects[objName] = {name: objName, sections: {}};
		}
		objects[objName].sections[names[1]] = 1;
	    }
	    else {
		//print("warning: skipped " + k);
	    }
        }
	if (!isLib) {
	    objName = c.name.split(/\s/)[0];
	    objects[objName] = {name: objName, sections: {}};
	}
    }

    /* run nm on all top-level libraries and objects */
    for (var i = 0; i < carray.length; i++) {
        var c = carray[i];
	var tokens = c.name.match(/([a-zA-Z-0-9+_\.]+)\s\(([a-zA-Z-0-9_+\.\\\/]*)\)/);
	if (tokens == null) {
	    print("can't parse " + c.name);
	    continue;
	}
	/* skip over internal section names */
	if (tokens[1] == "COMMON") {
	    continue;
	}

	var prefix = (tokens[2] == "" || tokens[2] == null) ? "." : tokens[2];
	var fileName = prefix + '/' + tokens[1];

        if (java.io.File(fileName).exists()) {

	    /* run nm */
	    var options = {useEnv: true};
	    var result = {};
	    xdc.exec(["nm", fileName], options, result);
	    if (result.exitStatus != 0) {
		print(result.output);
		java.lang.System.exit(result.exitStatus);
	    }

	    /* parse the output of nm */
	    var header = "  nm " + tokens[1] + " ...";
	    var lines = result.output.split(/[\n\r]+/);
	    var curObj = tokens[1];
	    for (var j = 0; j < lines.length; j++) {
		var line = lines[j];

		/* skip over object file name lines (from "nm <archive>") */
		var tokens = line.match(/^([a-zA-Z-0-9_+\.\\\/]*):$/);
		if (tokens && tokens[1]) {
		    curObj = tokens[1];
		    continue;
		}

		/* skip over curObj if it's not in the executable */
		if (objects[curObj] == null) {
		    continue;
		}

		/* if the line matches a reference, print it */
		if (line.match(name)) {
		    if (header != null) {
			print(header);
			header = null;
		    }
		    print("    " + curObj + " : " + line + "\n");
		}
	    }
	}
	else {
	    print("can't find " + fileName + " (" + c.name + ")");
	}
    }
}

/*
 *  ======== display ========
 *  Print the array of values returned by parse()
 */
function display(carray, fileName, verbose)
{
    var total = 0;
    var pad = "       ";
    for (var i = 0; i < carray.length; i++) {
        var c = carray[i];
        var len = String(c.total).length;
        print("  " + c.total + pad.substring(len) + c.name);
        total += c.total;

        if (verbose) {
            /* create a sections array and print it in size order */
            var sections = [];
            for (var k in c.sections) {
                sections.push({name: k, total: c.sections[k]});
            }
            sections.sort(function (a, b) {return (b.total - a.total);});
            var nlines = verbose > 1 ? sections.length : Math.min(5, sections.length);
            for (var j = 0; j < nlines; j++) {
                len = String(sections[j].total).length;
                print("    " + sections[j].total + pad.substring(len) + sections[j].name);
            }
        }
    }

    var len = String(total).length;
    print("  " + total + pad.substring(len) + "TOTAL: " + fileName);

    var targ = getTargetSuffix(carray);
    
    /* display unused SRAM and FLASH */
    displayMem("SRAM", fileName, targ);
    displayMem("FLASH", fileName, targ);
}

/*
 *  ======== displayMem ========
 */
function displayMem(name, fileName, targ)
{
    var pad = "       ";
    targ = (targ == null) ? "" : (", targ = " + targ);
    
    /* display unused memory named 'name' */
    var start = symbolTable[name + "_UNUSED_start"];
    var end = symbolTable[name + "_UNUSED_end"];
    var len = symbolTable[name + "_length"];
    if (start != null && end != null) {
	var unused = end - start;
	var suffix = "";
        if (len != null) {
	    var percent = len == 0 ? 100 : (unused / len) * 100;
	    suffix = " (" + percent.toFixed(0)
                      + "%, used = " + (len - unused) + targ + ")";
        }

        len = String(unused).length;
        print("  " + unused + pad.substring(len) + "UNUSED "
              + name + suffix + ": " + fileName);
    }
}
