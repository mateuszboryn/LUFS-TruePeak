/*
  =================================================================

  This file is part of the LUFS-TruePeak program.
  Copyright (c) 2015 - Mathieu Pavageau - contact@repetito.com

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  Details of these licenses can be found at: www.gnu.org/licenses

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  =================================================================
*/

#pragma once 

class Patch 
{
public:
    Patch();

    ~Patch();

    void setColumnNames(const juce::StringArray & columnNames);

    void setLineNames(const juce::StringArray & columnNames);

    void initFromXml(const juce::XmlElement * state);

    juce::XmlElement* createStateXml(const juce::String & patchString) const;

    void audioDeviceAboutToStart(juce::AudioIODevice* device);

    const juce::AudioSampleBuffer getBuffer(float ** channelData, int sampleCount);

    bool getState(int column, int line) const;

    void setState(int column, int line, bool state);

    juce::BigInteger getActiveColumns() const;
    juce::BigInteger getActiveLines() const;
    juce::BigInteger getActiveLines(const juce::String & deviceTagName) const;
    juce::BigInteger getActiveLines(const juce::String & deviceType, const juce::String & deviceName) const;

    int getIndex(int column, int line) const;

    const juce::StringArray & getColumnNames() const { return m_columnNames; }
    const juce::StringArray & getLineNames() const { return m_lineNames; }

private:

    juce::String generateDeviceTagName(const juce::String & deviceType, const juce::String & deviceName) const;

    const juce::BigInteger getDevicePatch(const juce::String & deviceTagName) const; // gets device patch associated with deviceTagName

    const juce::BigInteger getValidDevicePatch(const juce::String & deviceType, const juce::String & deviceName, int columnCount); // gets device patch and checks that it complies with columnCount

    juce::String m_deviceTagName;
    juce::NamedValueSet m_devicePatches;

    juce::StringArray m_columnNames;
    juce::StringArray m_lineNames;

    juce::BigInteger m_patch;
    juce::AudioSampleBuffer m_buffer; // used as silence buffer for inputs, scratch buffer for outputs

    bool m_dirty;

    float ** m_floatArray;
    int * m_patchArray; 
    int m_arraySize;
    int m_inputChannelCount;
};

class PatchButton;

class PatchView : public juce::Viewport
{
public:

    class Component 
        : public juce::Component
        , public juce::Button::Listener
    {
    public:

        class Listener
        {
        public:
            virtual void patchHasChanged(const juce::BigInteger & activeLines, const juce::BigInteger & activeColumns) = 0;
        };

        Component(Patch & patch, int columnXInc, int lineYInc, int arrayX, bool enableMultiSelection);

        virtual ~Component(); 

        void paint( juce::Graphics & g ) override;

        // juce::Button::Listener
        void buttonClicked(juce::Button * button) override;
        void buttonStateChanged(juce::Button * button) override;

        void redraw();
        void deleteButtons();

        void addListener(Component::Listener * listener) { m_listeners.add(listener); }

    private:

        juce::Array<PatchButton*> m_buttonArray;

        juce::Array<Listener*> m_listeners;

        Patch & m_patch;
        int m_columnXInc;
        int m_lineYInc;
        int m_arrayX;
        bool m_enableMultiSelection;
    };

    PatchView(Patch & patch, int columnXInc, int lineYInc, int arrayX, bool enableMultiSelection);

    void redraw() { m_component.redraw(); }

    void addListener(Component::Listener * listener) { m_component.addListener(listener); }

private:

    Component m_component;
};

