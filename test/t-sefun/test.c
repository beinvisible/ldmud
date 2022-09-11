#include "/inc/deep_eq.inc"
#include "/inc/msg.inc"
#include "/inc/testarray.inc"

#include "/sys/driver_info.h"

/* A struct with the same name as an sefun. */
struct sefunF001 {};

mixed* fun(string arg, int num)
{
    return ({arg, num});
}

void run_test()
{
    msg("\nRunning test for simul-efuns:\n"
          "-----------------------------\n");

    return run_array(({
        ({ "Check number of tabled sefuns", 0,
            (:
                return driver_info(DI_NUM_SIMUL_EFUNS_TABLED) == 65534;
            :)
        }),
        ({ "Calling sefun0000", 0,
            (:
                return sefun0000() == 0;
            :)
        }),
        ({ "Calling sefunF000", 0,
            (:
                return sefunF000() == 0xf000;
            :)
        }),
        ({ "Calling sefunF001", 0,
            (:
                return sefunF001() == 0xf001;
            :)
        }),
        ({ "Calling sefunFFFF", 0,
            (:
                return sefunFFFF() == 0xffff;
            :)
        }),
        ({ "Calling #'sefun0000", 0,
            (:
                return funcall(#'sefun0000) == 0;
            :)
        }),
        ({ "Calling #'sefunF000", 0,
            (:
                return funcall(#'sefunF000) == 0xf000;
            :)
        }),
        ({ "Calling #'sefunF001", 0,
            (:
                return funcall(#'sefunF001) == 0xf001;
            :)
        }),
        ({ "Calling #'sefunFFFF", 0,
            (:
                return funcall(#'sefunFFFF) == 0xffff;
            :)
        }),
        ({ "Calling symbol_function(\"sefun0000\")", 0,
            (:
                return funcall(symbol_function("sefun0000")) == 0;
            :)
        }),
        ({ "Calling symbol_function(\"sefunF000\")", 0,
            (:
                return funcall(symbol_function("sefunF000")) == 0xf000;
            :)
        }),
        ({ "Calling symbol_function(\"sefunF001\")", 0,
            (:
                return funcall(symbol_function("sefunF001")) == 0xf001;
            :)
        }),
        ({ "Calling symbol_function(\"sefunFFFF\")", 0,
            (:
                return funcall(symbol_function("sefunFFFF")) == 0xffff;
            :)
        }),
        ({ "Calling increment()", 0,
            (:
                return increment(100) == 102;
            :)
        }),
        ({ "Calling simul-efun call_strict", 0,
            (:
                object ob = this_object();
                return deep_eq(ob.fun(10), ({"sefun", 10}));
            :)
        }),
        ({ "Calling simul-efun call_other", 0,
            (:
                object ob = this_object();
                return deep_eq(ob->fun(20), ({"sefun", 20}));
            :)
        }),
        ({ "Calling simul-efun with optional arguments", 0,
            (:
                return opt_args_sefun(1, 3) == 531;
            :)
        }),
        ({ "Calling simul-efun closure with optional arguments", 0,
            (:
                return funcall(#'opt_args_sefun, 1, 3) == 531;
            :)
        }),
        ({ "Lambda: Calling simul-efun with optional arguments", 0,
            lambda(0, ({#'==, ({#'opt_args_sefun, 1, 3}), 531})),
        }),
        ({
           "Loading master using simul-efun closure", 0,
           (:
               object ob = find_object("/master");
               rename("/master.c", "/master-old.c");
               copy_file("/master-new.c", "/master.c");

               destruct(ob);

               catch(master());
               rm("/master.c");
               rename("/master-old.c", "/master.c");

               return find_object("/master").is_old_master();
           :)
        }),
        ({
           "Loading master using simul-efun struct", 0,
           (:
               object ob = find_object("/master");
               rename("/master.c", "/master-old.c");
               copy_file("/master-new-struct.c", "/master.c");

               destruct(ob);

               catch(master());
               rm("/master.c");
               rename("/master-old.c", "/master.c");

               return find_object("/master").is_old_master();
           :)
        }),
        ({ "Cleanup of struct definitions after compilation 1", TF_ERROR,
           (:
               load_object("/struct1");
           :)
        }),
        ({ "Cleanup of struct definitions after compilation 2", 0,
           (:
               load_object("/struct2");
               return 1;
           :)
        }),
        ({ "Multiple uses of simul-efun struct.", 0,
           (:
               foreach (int i: 100)
               {
                   object ob = load_object("/struct3");
                   ob->run_test();
                   destruct(ob);
               }
               return 1;
           :)
        }),
    }) + map(get_dir("/tl-*.c"), function mixed* (string fname)
    {
        return ({ sprintf("Testing '%s'", fname[..<3]), 0,
            (:
                int res;

                return !catch(res = load_object(fname[..<3])->run_test()) && res;
            :)
        });
    }) + map(get_dir("/tf-*.c"), function mixed* (string fname)
    {
        return ({ sprintf("Testing '%s'", fname[..<3]), 0,
            (:
                return catch(load_object(fname[..<3])) != 0;
            :)
        });
    }), #'shutdown);
}
