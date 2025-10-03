# README

Assignment A1 in the course Computer Systems @ DIKU, UCPH

Asger Ussing - ctz435 <br> Sebastian Andreassen - bwj836 <br> Victor Panduro Andersen - xkf701

<https://github.com/SebastianAndreassen/A1---Dynamic-Memory-and-Cache-Optimisations>

---

**To run any of the programs:**

1. Download the full repository to your client.
2. Navigate to the src folder in your terminal.
3. Run the command:

    ~~~bash
    make all
    ~~~

4. Choose the program you want to run, and then run the command:

    ~~~bash
    ./<program-name> 20000records.tsv
    ~~~

    Where you put the executable in \<program-name\>

---

**To run the test script:**

1. Follow step 1-3 from above.
2. Run the command:

    ~~~bash
    ./run_tests.sh
    ~~~

3. Locate and view the "valgrind_output.txt" file in the src folder.

If you do not have the permission to run the test script, the run the command:

~~~bash
chmod +x run_test.sh
~~~

And then rerun 2.
