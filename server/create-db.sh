#!/bin/bash
echo "CREATE DATABASE fast_typing;" | sudo -u postgres psql
export FASTTYPING_DB=fast_typing
