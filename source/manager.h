#pragma once

template <typename T>
class SingleMgr // 单例管理器模板类
{
public:
    static T* GetInstance()
    {
        if (!m_pInstance)
        {
            m_pInstance = new T();
        };
        return m_pInstance;
    };
private:
    static T* m_pInstance;
protected:
    SingleMgr() = default;
    ~SingleMgr() = default;
    SingleMgr(const SingleMgr&) = delete;
    SingleMgr& operator=(const SingleMgr&) = delete;
};

template <typename T>
T* SingleMgr<T>::m_pInstance = nullptr; // 静态成员变量初始化