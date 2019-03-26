#!/bin/bash

cd ./client/csharp

nuget restore cs-pico.sln

cd ../..

ls -al ./client/csharp/packages