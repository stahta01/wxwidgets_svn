///////////////////////////////////////////////////////////////////////////////
// Name:        wx/uianimation.h
// Purpose:     Declaration of animation/keyframe/keyframe animation classes
// Author:      Chiciu Bogdan Gabriel
// Created:     2011-05-11
// RCS-ID:      $Id$
// Copyright:   (c) 2011 wxWidgets Team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UI_ANIMATION_H_BASE_
#define _WX_UI_ANIMATION_H_BASE_

#ifdef __WXCOCOA__
    class wxString;
    // TODO: add includes for NSValue/etc
#endif

// Supported easing functions.
enum wxAnimationCurve
{
    wxANIMATION_CURVE_EASE_IN,
    wxANIMATION_CURVE_EASE_OUT,
    wxANIMATION_CURVE_EASE_IN_OUT,
    wxANIMATION_CURVE_LINEAR
};

// Supported target properties.
enum wxAnimationTargetProperty
{
    wxANIMATION_TARGET_PROPERTY_BACKGROUND_COLOR,
    wxANIMATION_TARGET_PROPERTY_POSITION,
    wxANIMATION_TARGET_PROPERTY_OPACITY,
    wxANIMATION_TARGET_PROPERTY_SIZE,
    wxANIMATION_TARGET_PROPERTY_HIDDEN
};

// Type enforcing helper for animation properties. These structs will restrict usage of a wrong type
// for an animation property (such as double for background colour). They also provide a convenient
// use for returning the OS X property path string (such as 'position' for wxANIMATION_TARGET_PROPERTY_POSITION)
// and to provide the conversion from the accepted property type to the OS X required equivalent (such as NSPoint for
// an animation that targets the position of a control).
template <wxAnimationTargetProperty N> struct wxPropertyType;
template<> struct wxPropertyType<wxANIMATION_TARGET_PROPERTY_BACKGROUND_COLOR> 
{
    typedef wxColour ValueType;
#if __WXCOCOA__
    static const wxString OsXKeyPath;
    static CGColor GetOsXValue(wxColour value)
    {
        UIColor color = [UIColor colorWithRed: value.Red() green: value.Green() blue: value.Blue() alpha: 1.0f];
        return color.CGColor;
    }
#endif
};

template<> struct wxPropertyType<wxANIMATION_TARGET_PROPERTY_POSITION> 
{
    typedef wxPoint ValueType;
#if __WXCOCOA__
    static const wxString OsXKeyPath;
    static NSValue GetOsXValue(wxPoint value)
    {
        return [NSValue valueWithPoint:(value.x, value.y);
    }
#endif
};

template<> struct wxPropertyType<wxANIMATION_TARGET_PROPERTY_OPACITY> 
{
    typedef int ValueType;
#if __WXCOCOA__
    static const wxString OsXKeyPath;
    static NSNumber GetOsXValue(int value)
    {
        return [NSNumber numberWithFloat:(value/100.0f);
    }
#endif
};

template<> struct wxPropertyType<wxANIMATION_TARGET_PROPERTY_SIZE> 
{
    typedef wxSize ValueType;
#if __WXCOCOA__
    static const wxString OsXKeyPath;
    static NSNumber GetOsXValue(wxSize value)
    {
        return [NSNumber numberWithFloat:(value/100.0f);
    }
#endif
};

template<> struct wxPropertyType<wxANIMATION_TARGET_PROPERTY_HIDDEN> 
{
    typedef bool ValueType;
#if __WXCOCOA__
    static const wxString OsXKeyPath;
    static NSNumber GetOsXValue(bool value)
    {
        return [NSNumber numberWithFloat:(value/100.0f);
    }
#endif
};

// Base animation class.
class WXDLLIMPEXP_ANIMATION wxUIAnimationBase
{
public:
    wxUIAnimationBase(double duration,
        wxAnimationTargetProperty targetProperty,
        wxAnimationCurve curve = wxANIMATION_CURVE_EASE_IN_OUT)
    {
        m_durationInSeconds = duration;
        m_targetProperty = targetProperty;
        m_curve = curve;
    }

    void SetDuration(double durationInSeconds)
    {
        m_durationInSeconds = durationInSeconds;
    }

    double GetDuration() const
    {
        return m_durationInSeconds;
    }

    void SetAnimationCurve(wxAnimationCurve curve)
    {
        m_curve = curve;
    }

    wxAnimationCurve GetAnimationCurve() const
    {
        return m_curve;
    }

    void SetTargetProperty(wxAnimationTargetProperty targetProperty)
    {
        m_targetProperty = targetProperty;
    }

    wxAnimationTargetProperty GetTargetProperty() const
    {
        return m_targetProperty;
    }

protected:
    double m_durationInSeconds;
    wxAnimationCurve m_curve;
    wxAnimationTargetProperty m_targetProperty;
};

// Basic property animationn class.
template <wxAnimationTargetProperty Property> class WXDLLIMPEXP_ANIMATION wxUIAnimation : public wxUIAnimationBase
{
public:
    typedef typename wxPropertyType<Property>::ValueType Type;

    // Default constructor.
    wxUIAnimation(Type initialValue,
        Type targetValue,
        double duration,
        wxAnimationCurve curve = wxANIMATION_CURVE_EASE_IN_OUT)
        : wxUIAnimationBase(duration, Property, curve)
    {
        m_initialValue = initialValue;
        m_targetValue = targetValue;
    }

    ~wxUIAnimation()
    {
    }

    // -----------------------------------------------------------
    // Animation properties.
    // -----------------------------------------------------------
    void SetInitialValue(Type initialValue)
    {
        m_initialValue = initialValue;
    }

    Type GetInitialValue() const
    {
        return m_initialValue;
    }

    void SetTargetValue(Type targetValue)
    {
        m_TargetValue = targetValue;
    }

    Type GetTargetValue() const
    {
        return m_targetValue;
    }
    
protected:
    Type m_initialValue;
    Type m_targetValue;
};

// Key frame class.
template<class T> class WXDLLIMPEXP_ANIMATION wxUIAnimationKeyframe
{
public:
    // -----------------------------------------------------------
    // Constructors.
    // -----------------------------------------------------------
    wxUIAnimationKeyframe(T value, double durationInSeconds)
        : m_value(value), m_durationInSeconds(durationInSeconds)
    {   
    }

    wxUIAnimationKeyframe(const wxUIAnimationKeyframe& copy)
    {
        m_value = copy.GetValue();
        m_durationInSeconds = copy.GetDuration();
    }

    virtual ~wxUIAnimationKeyframe()
    {
    }

    // -----------------------------------------------------------
    // Keyframe properties.
    // -----------------------------------------------------------
    void SetDuration(double durationInSeconds)
    {
        m_durationInSeconds = durationInSeconds;
    }

    double GetDuration() const
    {
        return m_durationInSeconds;
    }

    void SetValue(T value)
    {
        m_value = value;
    }

    T GetValue() const
    {
        return m_value;
    }

protected:
    double m_durationInSeconds;
    T m_value;
};

// Key frame animation class.
template<wxAnimationTargetProperty Property> class WXDLLIMPEXP_ANIMATION wxUIKeyframeAnimation : public wxUIAnimationBase
{
public:
    typedef typename wxPropertyType<Property>::ValueType ValueType;
    // Constructor.
    // TODO: resolve issue for key frame animation durtion
    wxUIKeyframeAnimation(double duration = 0, wxAnimationCurve curve = wxANIMATION_CURVE_EASE_IN_OUT)
        :wxUIAnimationBase(duration, Property, curve)
    {
    }

    virtual ~wxUIKeyframeAnimation()
    {
        m_keyframes.clear();
    }

    // -----------------------------------------------------------
    // Animation properties.
    // -----------------------------------------------------------
    void AddKeyframe(const wxUIAnimationKeyframe<ValueType>& keyframe)
    {
        m_durationInSeconds += keyframe.GetDuration();
        m_keyframes.push_back(keyframe);
    }

    const wxVector<wxUIAnimationKeyframe<ValueType> >& GetKeyframes() const
    {
        return m_keyframes;
    }

    //DECLARE_ABSTRACT_CLASS(wxUIAnimationBase)
protected:
    wxVector<wxUIAnimationKeyframe<ValueType> > m_keyframes; 
};

#endif //_WX_UI_ANIMATION_H_BASE_
