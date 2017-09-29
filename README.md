
TrustNode specific OpenWRT packages

install:
```{r, engine='bash', code_block_name}
echo "src-git InnoRouteTN https://github.com/InnoRoute/packages.git" >> feeds.conf.default
scripts/feeds update InnoRouteTN
scripts/feeds install -p InnoRouteTN
scripts/feeds install -p InnoRouteTN TrustNode
```

