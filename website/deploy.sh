#!/bin/bash
rm -rf ./public
snap run hugo -D -b https://www.f3.to/cellsol/
zip -r -9 cellsolpublic.zip ./public > zip.log
rm -rf ./public
ls -l public.zip

