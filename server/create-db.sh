#!/bin/bash
echo "CREATE DATABASE fast_typing;" | sudo -u postgres psql
export FASTTYOING_SB=fast_typing
