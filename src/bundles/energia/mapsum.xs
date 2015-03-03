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

function main(arguments)
{
    var toolChain = "ti";
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
    display(carray, verbose);
}

/*
 *  ======== parse ========
 */
function parse(fileName, toolChain)
{
    var result = null;

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
            throw new Error("unknown toolchain '" + toolChain + "'; use 'ti' or 'gnu'.");
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
    while ((line = file.readLine()) != null) {
	line = String(line);
        if (line.indexOf("Linker script and memory map") == 0) {
            header = false;
        }
        if (header) continue;

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
        var tokens = line.match(/^ ([\.a-zA-Z0-9_:\*]+)\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s+([\.a-zA-Z0-9_\/\\\-]+)(\(.+\))?/);
        if (tokens == null) {
            if (continuation == true) {
                continuation = false;
                tokens = line.match(/^ \s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s+([\.a-zA-Z0-9_\/\\\-]+)(\(.+\))?/);
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
                tokens = line.match(/^ ([\.a-zA-Z0-9_:\*]+)$/);
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
        key = key.substring(index + 1) 
	    + " (" + key.substring(0, index).replace(/\\/g, '/') + ")";

        /* accumulate sizes for each "container" */
        if (result[key] == null) {
            result[key] = {name: key, total: 0, sections: {}};
        }
        result[key].total += size;

        /* accumulate sizes of each contained section */
        var ssize = result[key].sections[section];
        result[key].sections[section] = size + (ssize != null ? ssize : 0);
    }

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
    var tokens = line.match(/^\s+(0x[a-fA-F0-9]+)\s+([a-zA-Z_]+[a-zA-Z0-9_]*)/);
    if (tokens != null && tokens[1] != null && tokens[2] != null) {
	if (symbolNames[tokens[2]] != null) {
	    symbolTable[symbolNames[tokens[2]]] = Number(tokens[1]);
	    return (true);
	}
    }
    return (false);
}

/*
 *  ======== display ========
 *  Print the array of values returned by parse()
 */
function display(carray, verbose)
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
    print("  " + total + pad.substring(len) + "TOTAL");

    /* display unused SRAM and FLASH */
    displayMem("SRAM");
    displayMem("FLASH");
}

/*
 *  ======== displayMem ========
 */
function displayMem(name)
{
    var pad = "       ";

    /* display unused memory named 'name' */
    var start = symbolTable[name + "_UNUSED_start"];
    var end = symbolTable[name + "_UNUSED_end"];
    var len = symbolTable[name + "_length"];
    if (start != null && end != null) {
	var unused = end - start;
	var suffix = len != null 
	    ? (" (" + unused + "/" + len + ", used = " + (len-unused) + ")") 
	    : "";
	len = String(unused).length;
	print("  " + unused + pad.substring(len) + "UNUSED " + name + suffix);
    }
}
