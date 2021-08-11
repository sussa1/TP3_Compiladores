echo "Testes Case List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser case_list/t$testname.cl > test1.results
  ./myparserCorrecto case_list/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Class List"

for i in {1..3..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser class_list/t$testname.cl > test1.results
  ./myparserCorrecto class_list/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Expression"

for i in {1..2..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser expression/t$testname.cl > test1.results
  ./myparserCorrecto expression/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Expression Let"

for i in {1..6..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser expression_let/t$testname.cl > test1.results
  ./myparserCorrecto expression_let/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Expression List Comma"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser expression_list_comma/t$testname.cl > test1.results
  ./myparserCorrecto expression_list_comma/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Expression List Semicolon"

for i in {1..3..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser expression_list_semic/t$testname.cl > test1.results
  ./myparserCorrecto expression_list_semic/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Feature List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser feature_list/t$testname.cl > test1.results
  ./myparserCorrecto feature_list/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Formal List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser formal_list/t$testname.cl > test1.results
  ./myparserCorrecto formal_list/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2
