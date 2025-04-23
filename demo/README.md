# Demo build

## Build
```shell
cmake -B build
make -C build
```

## Lägga till filer

- skapa ny .c fil i src/.
- headers läggs in i include/.
- lägg till .c filen i target_sources delen i CMakeLists.txt.

## TODO

- [ ] Photoresistor
- [ ] Fuktighetsensor
- [ ] Display-funktionalitet

## commit messages cheatsheets

```shell
feat: det här är en ny feature, använd vid ny funktionalitet
fix: det här är en fix/bugg commit, använd när man fixat/lagat något
refactor: det här är bara när man skrivit om kod men funktionaliteten är desamma
style: commits som inte ändrar något meningsfullt, t.ex. formattering
docs: uppdatering av dokumentation
build: ändring av byggdelen
chore: ändring av t.ex. config filer, .gitignore
```
