#!/bin/bash
name="TN_MMI"
message=${1:-"$name sync from internal repro"}
gitdir="/home/marian/Repros/packages/$name"
hgdir="/home/marian/Repros/trustnode_software/BuildRootPKGs/InnoRoute/$name"
cd $gitdir
for file in $(git ls-files); do
cp $hgdir/$file $gitdir/$file
git add $gitdir/$file
done
git commit -m"$message"
