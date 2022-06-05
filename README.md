![image-alt-name](./logo.png)

[![Continuous Integration](https://github.com/khbminus/fastTyping/actions/workflows/ci.yml/badge.svg)](https://github.com/khbminus/fastTyping/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/khbminus/fastTyping)

Тренажер печати с возможностью соревновательной игры

## О проекте

FASTYTPIGN - клиент-серверное приложение, позволяющее вам попрактиковаться в печати,
измерить WPM и посоревноваться с друзьями в быстром наборе текстов.

## Зависимости

- В данном проекте для клиента используется Qt версии не меньше 6. Разработка и тестирование производились на Qt 6.2.0.
- Для севера, в свою очередь, потребуется Boost.
- Также для сервера потребуется [libpqxx](https://github.com/jtv/libpqxx).
- Последней зависимостью является [nlohmann/json](https://github.com/nlohmann/json).

## Установка

### Установщик

Для установки клиента можно использовать комплектный установщик (Linux)

### Ручная установка

```sh
git clone https://github.com/khbminus/fastTyping.git
mkdir build
cd build
cmake .. -DBUILD_ALL=ON -DBUILD_CLIENT=OFF -DBUILD_SERVER=OFF # в зависимости от того, что вы хотите собрать назначив флаги 
make server
make client
make install # опционально
```

## Необходимые для работы файлы

Для запуска и корректной работы клиента в рабочей директории с проектом должны находиться файлы раскладок: `qwerty.json`
, `colemak.json`, `dvorak.json`.

Для корректной работы сервера требуется также собрать и выполнить цель `dictionaries-interface`, которая инициализирует
словари. Все текстовые словари должны находиться в рабочей папке в `dicts/`, а DLL-словари просто в корневой папке. 

