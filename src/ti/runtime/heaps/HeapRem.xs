/*
 *  ======== module$use ========
 */
function module$use()
{
    xdc.useModule("xdc.runtime.Error");
    xdc.useModule("xdc.runtime.Memory");
    xdc.useModule("xdc.runtime.Startup");
    xdc.useModule("ti.sysbios.heaps.HeapMem");
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj)
{
    obj.count = 0;
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(obj, mod)
{
    obj.heap = null;
}
