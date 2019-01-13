#ifndef __PROPERTY_H__
#define __PROPERTY_H__

// Ù–‘
#define GETSET_PROPERTY(TYPE_NAME, VAR_NAME, DEFAULT_VALUE) \
    public: \
        TYPE_NAME& Get_VAR_NAME(){ return VAR_NAME_; } \
        void Set_VAR_NAME(TYPE_NAME& value){ VAR_NAME_ = value; } \
    private: \
        TYPENAME VAR_NAME_{DEFAULT_VALUE};

#define READONLY_PROPERTY(TYPE_NAME, VAR_NAME, DEFAULT_VALUE) \
    public: \
        TYPE_NAME& Get_VAR_NAME(){ return VAR_NAME_; } \
    private: \
        TYPENAME VAR_NAME_{DEFAULT_VALUE};

#endif // __PROPERTY_H__
