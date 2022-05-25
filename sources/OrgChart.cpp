#include "OrgChart.hpp"
#include <stdexcept>


namespace ariel{

    OrgChart::OrgChart() : root(nullptr) {} // Init root with null as there's no root

    OrgChart& OrgChart::add_root(const std::string &_value) {
        if (_value.empty()){ // Cannot write _value == "". Suggested _value.empty()
            throw std::invalid_argument("> Cannot insert a nameless root!");
        }
        if(root == nullptr){            // Root does not exist 
            root = new Node(_value);
        }else{                          // Root already exist so just rename
            root->value = _value;
        }
    
        return *this;
    }

    OrgChart &OrgChart::add_sub(const std::string &father, const std::string &son) {
        // Search for the specific node you want to attach another node to it
        Node *father_node = search_for(father, root);
        
        // The node above haven't been found? throw an exception,
        // else push the attached-to-be node to the found node above
        father_node == nullptr ? throw std::invalid_argument("> " + father + "'s node does not exist") :
         father_node->adjacent.push_back(new Node(son));
        
        return *this;
    }
    
    OrgChart::~OrgChart() {
        // Destruct in this function
        recursive_destructor(root);
    }

    std::ostream &operator<<(std::ostream &os, const OrgChart &chart) {
        // Use helping function
        OrgChart::print_chart_wrap(os,chart.root);
        return os;
    }
      
    // Iterator implementation -------------------------
    OrgChart::Iterator::Iterator() : node(nullptr) {} // Initialtize default constructor

    // Operators of iterators class
    #pragma region Operators of iterators class
    std::string *OrgChart::Iterator::operator->() const {
        return &node->value; // Returns reference of value
    }

    std::string &OrgChart::Iterator::operator*() const {
        return node->value; // Returns just a value
    }
    
    OrgChart::Iterator OrgChart::Iterator::operator++(int) {
        // Taken from mat assignment (task 3 with the lovely operators)
        Iterator current = *this;
        ++(*this);
        return current;
    }
    
    OrgChart::Iterator& OrgChart::Iterator::operator++() {
        if ((queue_order.empty()))   // Queue is empty
        {  
            node = nullptr;          // Node is null
        }else{                       // Queue is not empty
            // Increment by save current and pop it
            node = queue_order.front();
            queue_order.pop();
        }
        
        return *this;
    }

    bool OrgChart::Iterator::operator!=(const Iterator &op) const {
        return !(node == op.node); // Not equals
    }

    bool OrgChart::Iterator::operator==(const Iterator &op) const {
        return node == op.node;   // Equals
    }
    #pragma endregion


    // ------------------------Preorder & order & reverse order constructors
    #pragma region iterator constructors
    OrgChart::it_reverse_order::it_reverse_order(Node *_node) {
        // https://stackoverflow.com/questions/3610933/iterating-c-vector-from-the-end-to-the-beginning
        auto *tor = &queue_order;                   // Init new queue used for fifo
        auto *current_node = &node;                 // Node used for fifo
        std::queue<Node*> reverse_que;              // Queue to store all the reverse nodes
        std::stack<Node*> st;                       // Node stack to push revese and withdraw in reverse
        if (_node != nullptr)                       // Given node isn't null
        {                    
            reverse_que.push(_node);
            while (!reverse_que.empty())
            {
                Node* front = reverse_que.front();
                reverse_que.pop();
                st.push(front);
                // Copied from stackoverflow (linke above)
                for (auto idx = front->adjacent.rbegin(); idx != front->adjacent.rend(); idx++) {
                    reverse_que.push(*idx);
                }
            }
            while(!st.empty())
            {   // Now push the reverse queue into a normal one
                tor->push(st.top());
                st.pop();
            }
            *current_node = tor->front();
            // Pop it out
            tor->pop();
        }else{ // Given node is null
            *current_node = nullptr;
        }
    }

    OrgChart::it_level_order::it_level_order(Node *_node) {
        auto *tor = &queue_order;                   // Init new queue used for fifo
        auto *current_node = &node;                 // Node used for fifo
        std::queue<Node*> node_queue;               // Queue of nodes
        if (_node != nullptr) {                     // given node isn't null
            node_queue.push(_node);
            while (!node_queue.empty())             // Queue isn't empty 
            {            
                Node *current = node_queue.front(); // Current holds the front node
                node_queue.pop();                   // Pop it
                tor->push(current);                 // Push into the tor queue current
                for(auto& idx : current->adjacent)  // Push the node's adjacents
                { 
                    node_queue.push(idx);
                }
            }
            // Current node is the front one of tor queue
            *current_node = tor->front();
            tor->pop();
        }
        else // Given node is null ptr
        { 
            *current_node = nullptr;
        }
    }

    OrgChart::it_preorder::it_preorder(Node *_node_) {
        auto *current_node = &node;                 // Node used for fifo
        if (_node_ != nullptr)                      // Given node isn't null
        {
            push_preorder(_node_, &queue_order);             // 'Walk' recursively
            *current_node = queue_order.front();
            queue_order.pop();                             // Pop current node
        }else{                                              // Given node is null
            current_node = nullptr;
        }
    }

    #pragma endregion
    
    
    // -----------------------------Preorder & order & reverse order iterators
    #pragma region Preorder & order & reverse order wraping funcions
    OrgChart::it_level_order OrgChart::begin() const{
        if (this->root == nullptr){ // Root is null
            throw std::invalid_argument("> Root does not exist");
        }

        return it_level_order(this->root);
    }

    OrgChart::it_level_order OrgChart::end() const{
        this->root == nullptr ? throw std::invalid_argument("> Root does not exist") : nullptr;
        return nullptr; // Doesn't matter if I return node or nullptr it's the end of the iterator
    }


    OrgChart::it_preorder OrgChart::begin_preorder() const{
        if (this->root == nullptr){ // Root is null
            throw std::invalid_argument("> Root does not exist");
        }

        return it_preorder(this->root);
    }

    OrgChart::it_preorder OrgChart::end_preorder() const{
        this->root == nullptr ? throw std::invalid_argument("> Root does not exist") : nullptr;
        return nullptr; // Doesn't matter if I return node or nullptr it's the end of the iterator
    }
    
    OrgChart::it_level_order OrgChart::begin_level_order() const{
        if (this->root == nullptr){ // Root is null
            throw std::invalid_argument("> Root does not exist");
        }

        return it_level_order(this->root);
    }

    OrgChart::it_level_order OrgChart::end_level_order() const {
        this->root == nullptr ? throw std::invalid_argument("> Root does not exist") : nullptr;
        return nullptr; // Doesn't matter if I return node or nullptr it's the end of the iterator
    
    }


    OrgChart::it_reverse_order OrgChart::begin_reverse_order() const {
        if (this->root == nullptr){ // Root is null
            throw std::invalid_argument("> Root does not exist");
        }
        
        return it_reverse_order(this->root);
    }

    OrgChart::it_reverse_order OrgChart::reverse_order() const{
        this->root == nullptr ? throw std::invalid_argument("> Root does not exist") : nullptr;
        return nullptr; // Doesn't matter if I return node or nullptr it's the end of the iterator
    
    }
    #pragma endregion


    // ==================--------------------------------
    // Helping function ---------------------------------
    // ==================--------------------------------
    #pragma region Helping function

    void OrgChart::recursive_destructor(Node *root) {
        // Recursion breaking condition
        if (root == nullptr) {
            return;
        }
        // Recursivley delete every node
        for (auto& idx : root->adjacent){
            recursive_destructor(idx);
        }
        // The deletion here
        delete root;
    }
    
    void OrgChart::it_preorder::push_preorder(Node *node, std::queue<ariel::OrgChart::Node*> *tor) {
        tor->push(node);
        for (auto& idx : node->adjacent) 
        {   // Push recursively every node and its adjacent list
            push_preorder(idx, tor);
        }
    }

    /**
     * @brief Deep copy a tree an returns a new tree (literally new)
     * 
     * @param parent 
     * @return OrgChart::Node* 
     */
    OrgChart::Node *OrgChart::deep_copy_tree(Node *parent) {
        // Parent does not exist
        if (parent==nullptr){
            return nullptr;
        }
        // Init a new node
        Node *new_copy(new Node(parent->value));
        // Walk over the node adjacent list to make a new clone
        for(auto& idx : parent->adjacent){
            // Recusively push back it nodes
            new_copy->adjacent.push_back(deep_copy_tree(idx));
        }
        return new_copy;
    }

    void OrgChart::print_chart(std::ostream &os,const std::string& prefix, const OrgChart::Node* node, bool isLeft)
    {
        if( node != nullptr )
        {
            os << prefix;

            if (isLeft){
                os << "├──";
            }
            else{
                os << "└──";
            } 

            // print the value of the node
            os << node->value << std::endl;

            // enter the next tree level - left and right branch
            for(const auto& idx : node->adjacent){
                print_chart(os, prefix + (isLeft ? "│   " : "    "), idx, true);
            }
        }
    }

    void OrgChart::print_chart_wrap(std::ostream &os,const OrgChart::Node* node)
    {
        print_chart(os, "", node, false);    
    }

    OrgChart::Node *OrgChart::search_for(const std::string &_value,Node *root) {
        // https://stackoverflow.com/questions/245628/c-binary-search-tree-recursive-search-function
        // Root is either null or it is the root
        if (root==nullptr || root->value==_value){ 
            return root;
        }
        // Search recursively over all the nodes vectors
        for (auto& idx : root->adjacent) {
            Node *node = search_for(_value,idx);            
            if (node != nullptr) { // Found the node!
                return node;
            }
        }

        return nullptr; // Haven't found the node
    }

    #pragma endregion

}
