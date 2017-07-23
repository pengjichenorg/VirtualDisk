#pragma once

#include <iostream>
#include <memory>
#include <typeindex>

class Any {
public:
    /**
     * empty constructor
     */
    Any(void) : m_typeIndex(std::type_index(typeid(void))) {

    }

    /**
     * copy constructor
     */
    Any(Any& any) : m_ptr(any.clone()), m_typeIndex(any.m_typeIndex) {

    }

    /**
     * move constructor
     */
    Any(Any&& any) : m_ptr(std::move(any.m_ptr)), m_typeIndex(any.m_typeIndex) {

    }

    /**
     * any type constructor
     */
    template <typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type>
    Any(U&& value) : m_ptr(new Derived<typename std::decay<U>::type>(std::forward<U>(value))), m_typeIndex(std::type_index(typeid(typename std::decay<U>::type))) {

    }

    /**
     * check if value is empty
     * @return nullptr
     */
    bool isNull() const {
        return !bool(m_ptr);
    }

    /**
     * check if value could convert to a specify type
     * @return convertable
     */
    template <typename U>
    bool is() const {
        return m_typeIndex == std::type_index(typeid(U));
    }

    /**
     * convert value to a specify type
     * @return value with specify type
     */
    template <typename U>
    U& cast() {
        if(!is<U>()) {
            std::cout << "can not cast " << typeid(U).name() << " to " << m_typeIndex.name() << std::endl;
            throw std::bad_cast();
        }
        auto derived = dynamic_cast<Derived<U>*>(m_ptr.get());
        return derived->m_value;
    }

    /**
     * operator=
     */
    Any& operator= (const Any& any) {
        if(m_ptr == any.m_ptr) {
            return *this;
        }
        m_ptr = any.clone();
        m_typeIndex = any.m_typeIndex;
        return *this;
    }

private:
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;
    struct Base {
        virtual ~Base() {

        }
        virtual BasePtr clone() const = 0;
    };

    template <typename T>
    struct Derived : public Base {
        template <typename U>
        Derived(U&& value) : m_value(std::forward<U>(value)) {

        }
        BasePtr clone() const {
            return BasePtr(new Derived<T>(m_value));
        }
        T m_value;
    };

    BasePtr clone() const {
        if(m_ptr != nullptr) {
            return m_ptr->clone();
        }
        return nullptr;
    }

    BasePtr m_ptr;
    std::type_index m_typeIndex;
};
