#!/bin/bash
# Tested on Mac OS X 10.8.2 (12C60)
# GNU bash, version 3.2.48(1)-release (x86_64-apple-darwin12)
echo "Engaging Verification..."
if [ -x wordgenerator.out -a -x spellchecker.out ]; then
  start_time=`date +%s`
  ./wordgenerator.out | ./spellchecker.out > verificationresults.txt
  end_time=`date +%s`
  if [ -f verificationresults.txt ]; then
    results="$(cat verificationresults.txt | grep -o  'No Suggestions' | wc -l)"
    end_time=`date +%s`
    echo $results "occurrence(s) of 'No Suggestions'"
    echo "Verification Complete."
    echo "Execution time was `expr $end_time - $start_time` second(s)."
  else
    echo "Unable to open the verification results file!"
  fi
else
  echo "Unable to execute the word generator and/or the spell checker!"
fi
