# Hierarchical-Sketch

This is the repository for source codes of Hierarchical Sketch.

## Introduction
Per-flow size measurement has wide applications in data center networks, such as flow scheduling, anomaly detection, and traffic engineering. To fit the module into limited on-chip memory and catch up with the line speed, exiting work either uses bit sharing scheme to compress the flow information or requires complex calculations to screen out large flows, resulting in substantial accuracy loss or heavy memory accesses, especially failing to measure medium-sized flows accurately, which are meaningful for practical applications such as usage accounting and billing. In light of this, we propose a new Hierarchical Sketch that adopts a hybrid on-chip/off-chip architecture to detect and estimate the flows whose size is no less than the threshold t (threshold-t flows for short) to cover both large and medium-sized flows. In Hierarchical Sketch, we design an on-chip bucket array together with an adaptive update scheme to record potential candidates for threshold-t flows within one memory access as well as filter out non-interest flows. Besides, the flow records are downloaded to off-chip space for high memory efficiency and online queries. We also implemented a prototype on the NetFPGA development platform. Experimental results show that Hierarchical Sketch is up to 72.61% more accurate and 245% faster than the best baseline.

## About this repo

### CPU

We implement and evaluate all algorithms on a server equipped with two Intel Xeon E5-2643 v4 @3.40GHz CPU and 256GB RAM.

### FPGA

We also implement our algorithm on a NetFPGA-1G-CML development platform featuring a Xilinx Kintex-7 XC7K325T FPGA.
