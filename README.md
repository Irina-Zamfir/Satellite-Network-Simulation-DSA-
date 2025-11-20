# Satellite-Network-Simulation DSA

This project implements a hierarchical satellite communication network using binary trees, min-heaps, and multi-way trees to simulate space-based data transmission and routing.

Technologies: C Programming, Binary Trees, Min-Heap, Multi-way Trees, BFS/DFS Algorithms, Dynamic Memory Management, Pointer-based Data Structures

## Main Features:

- Network construction and display

- Binary message decoding

- Binary message encoding

- Finding common parent satellite for a list of satellites (Requirement 4)

- Network expansion by connecting multi-way trees and calculating distances between any two satellites (Requirement 5)

## Data Structures:

- satellite: Structure representing a satellite in the network

- name: satellite designation

- freq: satellite frequency

- mother: link to parent

- nr_copii: number of child satellites

- children: vector of child satellites

- queue: Auxiliary structure for BFS traversal and display

## Key Functions:

- compare(): Compares satellites by frequency and name

- insert()/extract(): Heap insertion and extraction functions

- heapify(): Maintains heap property

- print(): Displays satellite network by levels using BFS

- decodificare(): Decodes binary messages by traversing from root

- find_path(): Encodes message path from root to target satellite

- bfs(): Breadth-first search to find satellites by name

- find_level(): Returns satellite level (root at level 0)

- find_common_satelite(): Finds common parent of two satellites

- find_distance(): Calculates distance between two satellites

- free_tree(): Recursively frees memory for entire network

## Implementation:

The program builds a satellite network using a min-heap. It reads satellite data, constructs the binary tree network through heap operations, and provides functionality for message encoding/decoding, parent finding, network expansion with multi-way trees, and distance calculation between satellites. Memory is managed dynamically with proper cleanup.
