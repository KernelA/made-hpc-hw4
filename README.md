# HW 2 умножение матриц и написание bash скриптов

## Требования для запуска

1. CMake 3.17 или выше.
2. Компилятор с поддержкой C++17.
3. Установленный BLAS.
4. ОС на основе UNIX.
5. Docker 19.04 или выше.

## Задания

### Исходный код проекта по перемножению матриц

Реализована два алгоритма: обычный и алгоритм Штрассена.

[См. matrix-mul (используется CMake вместо Makefile)](matrix-mul)

### Замеры времени

[При запуске в ОС](reports/native_os_test.txt)

[При запуске в Docker](reports/docker_test.txt)

### Скрипты для bash

[См. scripts](scripts/scripts.sh)

Результаты выполнения можно [посмотреть в GitHub Actions, в секции Test scripts](https://github.com/KernelA/made-hpc-hw2/actions/workflows/build.yaml)

### LINPACK тест

[Отчёт по запуску LINPACK теста от Intel](reports/linpack.txt)

[Была использована реализация теста от Intel](https://software.intel.com/content/www/us/en/develop/articles/intel-mkl-benchmarks-suite.html)

## Как запустить

Создать директорию: `build`.
```
mkdir build
cmake -S . -B ./build
```

Выполнить CMake:
```bash
cmake --build ./build --config Release --parallel $(nproc) --target all
```

После успешной сборки проекта:
```bash
docker build -t mult .
docker run mult
```

**Полна переносимость программы внутри контейнера не гарантируется при сборке на разных ОС**