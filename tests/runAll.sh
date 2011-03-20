#!/bin/bash
#
# This file would benefit from code duplication removal but there is no time
# left to do this. :) (And this is only for testing purposes)

tests=0
passed=0
failed=0

do_tests_learn() {
    tests=$(($tests+1))
    echo -ne "./id3 l atribute.txt invatare.txt out\t\t\t"
    ./id3 l $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 l -ndiv atribute.txt invatare.txt out\t\t"
    ./id3 l -ndiv $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 l -nfull atribute.txt invatare.txt out\t\t"
    ./id3 l -nfull $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_full_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 l -mmaj atribute.txt invatare.txt out\t\t"
    ./id3 l -mmaj $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 l -ndiv -mmaj atribute.txt invatare.txt out\t"
    ./id3 l -ndiv -mmaj $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 l -nfull -mmaj atribute.txt invatare.txt out\t"
    ./id3 l -nfull -mmaj $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_full_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 l -mprb atribute.txt invatare.txt out\t\t"
    ./id3 l -mprb $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_div_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 l -ndiv -mprb atribute.txt invatare.txt out\t"
    ./id3 l -ndiv -mprb $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_div_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 l -nfull -mprb atribute.txt invatare.txt out\t"
    ./id3 l -nfull -mprb $1/atribute.txt $1/invatare.txt $1/out
    diff $1/out $1/out_full_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi
}

do_tests_graph(){
    tests=$(($tests+1))
    echo -ne "./id3 g out graph #(div,maj)\t\t\t\t"
    ./id3 g $1/out_div_maj $1/out
    diff $1/out $1/out_ascii_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g out graph #(div,prb)\t\t\t\t"
    ./id3 g $1/out_div_prb $1/out
    diff $1/out $1/out_ascii_div_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g out graph #(full,maj)\t\t\t\t"
    ./id3 g $1/out_full_maj $1/out
    diff $1/out $1/out_ascii_full_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g out graph #(full,prb)\t\t\t\t"
    ./id3 g $1/out_full_prb $1/out
    diff $1/out $1/out_ascii_full_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g -gascii out graph #(div,maj)\t\t\t"
    ./id3 g -gascii $1/out_div_maj $1/out
    diff $1/out $1/out_ascii_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g -gascii out graph #(div,prb)\t\t\t"
    ./id3 g -gascii $1/out_div_prb $1/out
    diff $1/out $1/out_ascii_div_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g -gascii out graph #(full,maj)\t\t\t"
    ./id3 g -gascii $1/out_full_maj $1/out
    diff $1/out $1/out_ascii_full_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g -gascii out graph #(full,prb)\t\t\t"
    ./id3 g -gascii $1/out_full_prb $1/out
    diff $1/out $1/out_ascii_full_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g -gdot out | dot -Tpng > out.png #(div,maj)\t"
    ./id3 g -gdot $1/out_div_maj $1/out
    diff $1/out $1/out_dot_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi
    dot $1/out_dot_div_maj -Tpng > $1/out_dot_div_maj.png

    tests=$(($tests+1))
    echo -ne "./id3 g -gdot out | dot -Tpng > out.png #(div,prb)\t"
    ./id3 g -gdot $1/out_div_prb $1/out
    diff $1/out $1/out_dot_div_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi
    dot $1/out_dot_div_prb -Tpng > $1/out_dot_div_prb.png

    tests=$(($tests+1))
    echo -ne "./id3 g -gdot out | dot -Tpng > out.png #(full,maj)\t"
    ./id3 g -gdot $1/out_full_maj $1/out
    diff $1/out $1/out_dot_full_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi
    dot $1/out_dot_full_maj -Tpng > $1/out_dot_full_maj.png

    tests=$(($tests+1))
    echo -ne "./id3 g -gdot out | dot -Tpng > out.png #(full,prb)\t"
    ./id3 g -gdot $1/out_full_prb $1/out
    diff $1/out $1/out_dot_full_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi
    dot $1/out_dot_full_prb -Tpng > $1/out_dot_full_prb.png

    tests=$(($tests+1))
    echo -ne "./id3 g -gscheme out graph #(div,maj)\t\t\t"
    ./id3 g -gscheme $1/out_div_maj $1/out
    diff $1/out $1/out_scheme_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g -gscheme out graph #(div,prb)\t\t\t"
    ./id3 g -gscheme $1/out_div_prb $1/out_scheme_div_prb
    diff $1/out $1/out_scheme_div_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g -gscheme out graph #(full,maj)\t\t\t"
    ./id3 g -gscheme $1/out_full_maj $1/out
    diff $1/out $1/out_scheme_full_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 g -gscheme out graph #(full,prb)\t\t\t"
    ./id3 g -gscheme $1/out_full_prb $1/out
    diff $1/out $1/out_scheme_full_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi
}

do_tests_classify(){
    tests=$(($tests+1))
    echo -ne "./id3 c out test.txt result #(div,maj)\t\t\t"
    ./id3 c $1/out_div_maj $1/test.txt $1/out
    diff $1/out $1/out_test_div_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 c out test.txt result #(div,prb)\t\t\t"
    ./id3 c $1/out_div_prb $1/test.txt $1/out
    diff $1/out $1/out_test_div_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 c out test.txt result #(full,maj)\t\t\t"
    ./id3 c $1/out_full_maj $1/test.txt $1/out
    diff $1/out $1/out_test_full_maj &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

    tests=$(($tests+1))
    echo -ne "./id3 c out test.txt result #(full,prb)\t\t\t"
    ./id3 c $1/out_full_prb $1/test.txt $1/out
    diff $1/out $1/out_test_full_prb &> /dev/null
    if [ $? -eq 0 ]; then
        echo "passed"
        passed=$(($passed+1))
    else
        echo "failed"
        failed=$(($failed+1))
    fi

}

for d in tests/*; do
    if [ -d $d ]; then
        do_tests_learn $d
        do_tests_graph $d
        do_tests_classify $d
        rm -f $d/out
    fi
done

echo -ne "============ SUMMARY =============\n"
echo -ne "TESTS:\t\t\t$tests\n"
echo -ne "FAILED:\t\t\t$failed\n"
echo -ne "PASSED:\t\t\t$passed\n"
echo -ne "==================================\n"

