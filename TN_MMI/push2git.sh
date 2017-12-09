#!/bin/bash
name="TN_MMI"
message=${1:-"$name sync from internal repro"}
gitdir="/home/delphin/svn2/brynhild/InnoRoute/hg_sw/TrustNode/EmbeddedLinux/BuildRootPKGs/gitcontent/packages/$name"
hgdir="/home/delphin/svn2/brynhild/InnoRoute/hg_sw/TrustNode/EmbeddedLinux/BuildRootPKGs/InnoRoute/$name"
cd $gitdir
for file in $(git ls-files); do
cp $hgdir/$file $gitdir/$file
git add $gitdir/$file
done
git commit -m"$message"
