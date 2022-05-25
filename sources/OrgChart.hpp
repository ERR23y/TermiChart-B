/**
 * @file OrgChart.hpp
 * @author Ohad Maday (maday.ohad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>
#include <queue>

namespace ariel {
    /**
     * @brief OrgChart header file - Represent an hirerachy of roles/names
     * 
     */
    class OrgChart {    
    public:      
        // Inner classes first
        
        /**
         * @brief Node inner class which holds every title (CEO,CTO)
         * or even a person name
         * 
         */
        class Node {
            public:
            std::string value;                  // Value of each Node (has to be a string)
            std::vector<Node*> adjacent;        // Adjacent list of each node (Like a linked list of lists) 

            /**
             * @brief Construct a new Node object
             * 
             * @param _value 
             */
            Node(const std::string &_value) : value(_value) {} 
        };
        /**
         * @brief Inner class which holds the iterator and operates it. Comes with functions
         * 
         */
        class Iterator {
            public:
                Node *node;                    // Have to do a manipulation on Node object
                
                std::queue<Node*> queue_order; // Easier to implement using queue
                
                /**
                 * @brief Construct a new Iterator object
                 * 
                 */
                Iterator();

                /**
                 * @brief Construct a new copy Iterator object
                 * Default is so I won't get weird errors
                 */
                Iterator(Iterator const&) = default;

                // ----------------------------------------
                // Had to write these two (no implementation.... cause of weird tidy error)
                Iterator(Iterator &&) = default;

                Iterator &operator=(Iterator &&) = default;
                // -------------------------------------


                /**
                 * @brief Assumption operator
                 * 
                 * @return Iterator& 
                 */
                Iterator& operator=(Iterator const &) = default;

                /**
                 * @brief Operator *
                 * 
                 * @return std::string& 
                 */
                std::string& operator*() const;

                /**
                 * -------------------
                 * @brief Increment operators
                 * 
                 * @return Iterator 
                 */
                Iterator operator++(int);

                Iterator& operator++();
                // -------------------
                /**
                 * @brief Arrow operator (pointer operator)
                 * 
                 * @return pointer std::string* 
                 */
                std::string* operator->() const;

                /**
                 * @brief Boolean operators-----------------------
                 * 
                 * @param op 
                 * @return true 
                 * @return false 
                 */
                bool operator!=(const Iterator &op) const;

                bool operator==(const Iterator &op) const;

                virtual ~Iterator() = default;
                // ------------------------------------------------
        };

        /**
         * @brief Iterator class of preorder
         * 
         */
        class it_preorder : public Iterator {
            public:
                void push_preorder(Node *node, std::queue<ariel::OrgChart::Node *> *tor);
                it_preorder(Node *_node_);
        };

        /**
         * @brief Iterator class of reverse order
         * 
         */
        class it_reverse_order : public Iterator {
            public:
            it_reverse_order(Node *_node);
        };

        /**
         * @brief Iterator class of Level order
         * 
         */
        class it_level_order : public Iterator {
            public:
            it_level_order(Node *_node);
        };



        Node *root;  // Head of the chart

        /**
         * @brief Construct a new Org Chart object
         * 
         */
        OrgChart();

        // Had to add these signatures else make tidy would give me weird error... (Thanks to whatsapp c++ group for the solution )
        OrgChart(OrgChart &&node) noexcept;
        OrgChart &operator=(OrgChart &&node) noexcept;
        OrgChart &operator=(OrgChart const& node);
        OrgChart(const OrgChart& node);
        // ----------------------------------------------------


        
        /**
         * @brief Destroy *recursivly* the Org Chart object
         * uses recursive_destructor() function
         */
        ~OrgChart();

        OrgChart &add_root(const std::string &_value);

        OrgChart &add_sub(const std::string &father, const std::string &son);


        friend std::ostream &operator<<(std::ostream &os, const OrgChart &chart);

        /**
         * @brief Recursively delete nodes
         * 
         * @param root 
         */
        void recursive_destructor(Node *root); 

       
        Node *deep_copy_tree(Node *parent);

        /**
         * @brief Search recursivley for a node.
         * Used in add_sub function
         * https://stackoverflow.com/questions/245628/c-binary-search-tree-recursive-search-function
         * 
         * @param root 
         * @param v 
         * @return Node* 
         */
        Node *search_for(const std::string& _value,Node* root);


        /**
         * @brief Used in << operator function Helping function
         * taken from https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c#51730733
         * **Adjusted the code so it fit into this task**
         * @param os 
         * @param prefix 
         * @param node 
         * @param isLeft 
         */
        static void print_chart(std::ostream &os, const std::string& prefix, const Node* node, bool isLeft); 

        /**
         * @brief Wrapping function of the function above - Helping function
         * 
         * @param os 
         * @param node 
         */
        static void print_chart_wrap(std::ostream &os, const Node* node);

        /**
         * @brief Begin iterator function
         * 
         * @return it_level_order 
         */
        it_level_order begin() const;

        /**
         * @brief End iterator function
         * 
         * @return it_level_order 
         */
        it_level_order end() const;

        /**
         * @brief Begin reverse order iterator function
         * 
         * @return it_reverse_order 
         */
        it_reverse_order begin_reverse_order() const;

        /**
         * @brief End reverse order function
         * 
         * @return it_reverse_order 
         */
        it_reverse_order reverse_order() const;

        /**
         * @brief Begin level order function
         * 
         * @return it_level_order 
         */
        it_level_order begin_level_order() const;

        /**
         * @brief End level order function
         * 
         * @return it_level_order 
         */
        it_level_order end_level_order() const;
        
        /**
         * @brief Begin preorder iterator function
         * 
         * @return it_level_order 
         */
        it_preorder begin_preorder() const;
        
        /**
         * @brief End preorder iterator function
         * 
         * @return it_level_order 
         */
        it_preorder end_preorder() const;

    };
    /**
     * @brief Ostream operator
     * uses helping function - more documentation is found in source file
     * 
     * @param ost 
     * @param chart 
     * @return std::ostream& 
     */
    std::ostream &operator<<(std::ostream &ost, const OrgChart &chart);
    
}
