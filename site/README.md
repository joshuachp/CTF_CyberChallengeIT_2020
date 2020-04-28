# Sito Vulnerabile

Per creare il sito statico nella cartella `dist` utilizzare:

```bash
npm i
# Dev
npm run build 
# Prod
npm run prod
```

Serve però servirlo con un server, visto dei problemi di CORS con WAsm. Un
metodo facile può essere:

```bash
cd dist
python2 -m SimpleHTTPServer
```
