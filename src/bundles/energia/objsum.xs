/*
 *  ======== objsum.xs ========
 */

var usage = "usage: xs -c objsum.xs [-x <objdump_tool>] [-p <objdump_path>] [-v] object_file";

function main(arguments)
{
    var verbose = 0;
    var objdumpPath = "";
    var objdumpName = "objdump";
    
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
            case 'p': {
                if (arguments[1] == null) {
                    print(usage);
                    return;
                }
                objdumpPath += arguments[1] + java.io.File.pathSeparator;
                arguments.shift();
                break;
            }
	    case 'x': {
                if (arguments[1] == null) {
                    print(usage);
                    return;
                }
                objdumpName = arguments[1];
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
        return;
    }

    /* add objdump path to PATH, if necessary */
    var options = {useEnv: true};
    if (objdumpPath.length != 0) {
        options.envs = ["PATH=" + objdumpPath + java.lang.System.getenv("PATH")];
    }

    /* run objdump */
    var result = {};
    xdc.exec([objdumpName, "-h", arguments[0]], options, result);
    if (result.exitStatus != 0) {
	print(result.output);
        java.lang.System.exit(result.exitStatus);
    }

    /* parse the output from objdump */
    var sections = parse(result.output);

    /* output "standard" size information (text, data, bss, stack) */
    print(arguments[0] + " size summary:");
    var total = 0;
    var sums = {};
    for (var i = 0; i < sections.length; i++) {
        var size = sections[i];
        if (sums[size.type] == null) {
            sums[size.type] = 0;
        }
        sums[size.type] += sections[i].total;
    }
    for (k in sums) {
        print("  " + k + "        ".substring(k.length) + sums[k]);
        total += sums[k];
    }
    print("  TOTAL   " + total);

    /* output sizes of all sections that are loaded */
    if (verbose) {
        print("\n  Section Sizes");
        display(sections, verbose);
    }
}

/*
 *  ======== parse ========
 */
function parse(objdump)
{
    var sizes = {};
    var lines = objdump.split(/[\n\r]+/);
    for (var i = 0; i < lines.length; i++) {
        var line = lines[i];
        var tokens = line.match(/^\s*[0-9]+\s+([\.a-zA-Z0-9:_]+)\s+([0-9a-f]+)/);
        if (tokens != null) {
            var sname = tokens[1];
            var size = ("0x" + tokens[2]) - 0;
            var attrs = getAttrs(sname, lines[i + 1]);
            if (attrs == null || attrs.ALLOC != 1) {
                continue;
            }

            var key = sname.split(':')[0];
            if (sizes[key] == null) {
                sizes[key] = {name: key, type: attrs.type, total: 0, sections: {}};
            }
            sizes[key].total += size;
            sizes[key].sections[sname] = {total: size, attrs: attrs};
        }
    }

    /* create array and sort by size */
    var carray = [];
    for (var k in sizes) {
        carray.push(sizes[k]);
    }
    carray.sort(function (a, b) {return (b.total - a.total);});

    return (carray);
}

/*
 *  ======== getAttrs ========
 */
function getAttrs(name, line)
{
    var attrs = {};

    if (line == null) {
        return (null);
    }
    var ca = line.trim().split(/[\s,]+/);
    for (var i = 0; i < ca.length; i++) {
        attrs[ca[i]] = 1;
    }

    attrs.type = getType(name, attrs);

    return (attrs);
}

/*
 *  ======== getType ========
 */
function getType(name, attrs)
{
    if (attrs.ALLOC) {
        if (!attrs.CONTENTS) {
            return ("bss");
        }
        else if (attrs.DATA) {
            if (name.indexOf(".stack") == 0) {
                return ("stack");
            }
            else {
                return (attrs.READONLY ? "rodata" : "data");
            }
        }
        else if (attrs.CODE) {
            return ("text");
        }
        else {
            return ("misc");
            print(name + "is unclassified as text, bss, data, or stack");
        }
    }

    return ("noload");
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

        if (verbose > 1) {
            /* create a sections array and print it in size order */
            var sections = [];
            for (var k in c.sections) {
                sections.push({name: k, total: c.sections[k].total});
            }
            sections.sort(function (a, b) {return (b.total - a.total);});
            var nlines = verbose > 2 ? sections.length : Math.min(5, sections.length);
            if (nlines > 1)  {
                for (var j = 0; j < nlines; j++) {
                    len = String(sections[j].total).length;
                    print("    " + sections[j].total + pad.substring(len) + sections[j].name);
                }
            }
        }
    }
    len = String(total).length;
    print("  " + total + pad.substring(len) + "TOTAL");
}
