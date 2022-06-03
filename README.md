![image-alt-name](./logo.png)




Тренажер печати  с возможностью соревновательной игры

## О проекте

FASTYTPIGN - клиент-серверное приложение, позволяющее вам попрактиковаться в печати, 
измерить WPM и посоревноваться с друзьями в быстром наборе текстов.

## Установка

Для установки используйте команды: 
```sh
git clone https://github.com/khbminus/fastTyping.git
mkdir build
cd build
cmake .. cmake .. -DBUILD_ALL=ON -DBUILD_CLIENT=OFF -DBUILD_SERVER=OFF # в зависимости от того, что вы хотите собрать назначив флаги 
make server
make client
```


