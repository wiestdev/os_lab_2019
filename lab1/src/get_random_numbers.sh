#!/bin/bash
for i in {1..150}; do
	od -An -N2 -i /dev/random;
done
