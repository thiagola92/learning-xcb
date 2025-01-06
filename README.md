# learning-xcb
Quando lidando com X11 e XCB, muito conteúdo não está up-to-date (2025) ou possui um site muito agradável para novos desenvolvedores.  

Nos próprios sites é possível encontrar frases como:

>[TODO: These instructions are out of date. Just reference the main XCB page so we don't have to maintain these instructions in more than one place.](https://xcb.freedesktop.org/tutorial/#installingxcb)

>[TODO](https://xcb.freedesktop.org/tutorial/#thewindowhierarchy)

>[[XXX write me --po8]](https://www.x.org/wiki/guide/concepts/#inputviatouchscreen)

>[[Make whot write this? or steal from http://who-t.blogspot.com? --alanc]](https://www.x.org/wiki/guide/concepts/#advancedinputdevicesandtechniques)

Só consegui chegar em algumas referências mencionadas aqui por pesquisa no Google, seguindo os links do site não estava intuitivo.

**Dito isto, os códigos parecem ter funções bem documentadas então é importante sempre ler os docstrings!**  

## install
```
sudo apt install libx11-dev
```

## running example
```
gcc -o main main.c -lX11 -lxcb
```

## references
- https://www.x.org/wiki/guide/
- https://xcb.freedesktop.org/tutorial/
- https://who-t.blogspot.com/2010/11/high-level-overview-of-grabs.html